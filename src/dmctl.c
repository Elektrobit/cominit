// SPDX-License-Identifier: MIT
/**
 * @file dmctl.c
 * @brief Implementation of device-mapper setup.
 */
#include "dmctl.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <linux/dm-ioctl.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>

#include "meta.h"
#include "output.h"
#include "tpm.h"

#define cominitIoctlSetVersion(ioctlStruct)               \
    do {                                                  \
        (ioctlStruct).version[0] = DM_VERSION_MAJOR;      \
        (ioctlStruct).version[1] = DM_VERSION_MINOR;      \
        (ioctlStruct).version[2] = DM_VERSION_PATCHLEVEL; \
    } while (0)

/**
 * Structure for handling device-mapper ioctl operations.
 */
typedef struct cominitDmIoctlData {
    struct dm_ioctl ioctl;                  ///< Header structure used by all device-mapper ioctls.
    struct dm_target_spec tSpec;            ///< Specifications about the device mapper table.
    char dmTbl[COMINIT_DM_TABLE_SIZE_MAX];  ///< The device mapper table.
} cominitDmIoctlData_t;

/**
 * Get the sectors of a partition.
 *
 * Uses the BLKGETSIZE64 ioctl() to return the number of sectors.
 *
 * @param sectors  Return pointer for the number of sectors (512 bytes).
 * @param device      The device node associated to the partition.
 *
 * @return  0 on success, -1 otherwise
 */
static int cominitDmctlGetSectors(const char *device, unsigned long *sectors) {
    int fd = open(device, O_RDONLY | O_CLOEXEC);
    if (fd < 0) {
        return -1;
    }

    uint64_t partSize;
    if (cominitGetPartSize(&partSize, fd) == -1) {
        return -1;
    }

    *sectors = partSize / 512;
    close(fd);

    return 0;
}

/**
 * Check whether the algorithm for encryption is found.
 *
 * @param algorithm  The name of the algorithm to look for.
 *
 * @return  0 on success, -1 otherwise
 */
static int cominitDmctlCheckForCryptoAlg(const char *algorithm) {
    if (access("/proc/crypto", F_OK) != 0) {
        cominitErrnoPrint("/proc/crypto not mounted");
        return -1;
    }
    FILE *f = fopen("/proc/crypto", "r");
    if (!f) {
        cominitErrnoPrint("could not open /proc/crypto");
        return -1;
    }
    char line[256];
    int found = 0;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "name", 4) == 0) {
            char *separator = strrchr(line, ':');
            if (separator) {
                char *algorithmName = separator + 1;
                while (isspace((unsigned char)*algorithmName)) {
                    algorithmName++;
                }
                algorithmName[strcspn(algorithmName, "\n")] = '\0';

                if (strcmp(algorithmName, algorithm) == 0) {
                    found = 1;
                    break;
                }
            }
        }
    }
    fclose(f);
    if (found) {
        return 0;
    }

    return -1;
}

/**
 * Create a new device-mapper device.
 *
 * @param dmCtlFd  An open file descriptor to /dev/mapper/control.
 * @param dmi  Pointer to a cominitDmIoctlData_t structure.
 * @param name  The name of the new device-mapper device.
 *
 * @return  0 on success, -1 otherwise
 */
static int cominitDmctlCreateNewDmDevice(int dmCtlFd, cominitDmIoctlData_t *dmi, const char *name) {
    memset(&dmi->ioctl, 0, sizeof(dmi->ioctl));
    cominitIoctlSetVersion(dmi->ioctl);
    dmi->ioctl.data_size = sizeof(dmi->ioctl);
    strcpy(dmi->ioctl.name, name);

    return ioctl(dmCtlFd, (int)DM_DEV_CREATE, &dmi->ioctl);
}

/**
 * Start a new device-mapper device.
 *
 * @param dmCtlFd  An open file descriptor to /dev/mapper/control.
 * @param dmi  Pointer to a cominitDmIoctlData_t structure.
 * @param devId  The Id of the new device-mapper device.
 *
 * @return  0 on success, -1 otherwise
 */
static int cominitDmctlStartDmDevice(int dmCtlFd, cominitDmIoctlData_t *dmi, uint64_t devId) {
    memset(&dmi->ioctl, 0, sizeof(dmi->ioctl));
    cominitIoctlSetVersion(dmi->ioctl);
    dmi->ioctl.data_size = sizeof(dmi->ioctl);
    dmi->ioctl.dev = devId;
    // DM_SUSPEND_FLAG is unset, so DM_DEV_SUSPEND will resume.
    return ioctl(dmCtlFd, (int)DM_DEV_SUSPEND, &dmi->ioctl);
}

/**
 * Load a device-mapper table into the kernel.
 *
 * @param dmCtlFd  An open file descriptor to /dev/mapper/control.
 * @param dmi  Pointer to a cominitDmIoctlData_t structure.
 *
 * @return  0 on success, -1 otherwise
 */
static inline int cominitDmctlLoadDmTable(int dmCtlFd, cominitDmIoctlData_t *dmi) {
    return ioctl(dmCtlFd, (int)DM_TABLE_LOAD, &dmi->ioctl);
}

int cominitSetupDmDevice(cominitRfsMetaData_t *rfsMeta) {
    if (rfsMeta == NULL) {
        cominitErrPrint("Input parameter must not be NULL.");
        return -1;
    }

    const char *dmTgtStr;
    if (rfsMeta->crypt == COMINIT_CRYPTOPT_VERITY) {
        if (!rfsMeta->ro) {
            cominitErrPrint("A dm-verity target can only be opened read-only.");
            return -1;
        }
        dmTgtStr = "verity";
    } else if (rfsMeta->crypt == COMINIT_CRYPTOPT_INTEGRITY) {
        dmTgtStr = "integrity";
    } else {
        cominitErrPrint("Unsupported device mapper target.");
        return -1;
    }

    int dmCtlFd = open("/dev/" DM_DIR "/" DM_CONTROL_NODE, O_RDWR);
    if (dmCtlFd == -1) {
        cominitErrnoPrint("Could not open \'/dev" DM_DIR "/" DM_CONTROL_NODE "\'.");
        return -1;
    }

    cominitDmIoctlData_t dmi;
    if (cominitDmctlCreateNewDmDevice(dmCtlFd, &dmi, COMINIT_ROOTFS_DM_NAME) == -1) {
        cominitErrnoPrint("Could not create new device mapper device using ioctl().");
        close(dmCtlFd);
        return -1;
    }

    uint64_t devId = dmi.ioctl.dev;

    // Load dm-verity table to device mapper.
    memset(&dmi, 0, sizeof(dmi));
    cominitIoctlSetVersion(dmi.ioctl);
    dmi.ioctl.dev = devId;
    dmi.ioctl.flags = rfsMeta->ro ? DM_READONLY_FLAG : 0;
    dmi.ioctl.target_count = 1;
    dmi.ioctl.data_start = offsetof(cominitDmIoctlData_t, tSpec) - offsetof(cominitDmIoctlData_t, ioctl);
    dmi.tSpec.sector_start = 0;
    dmi.tSpec.length = rfsMeta->dmVerintDataSizeBytes / 512;
    strncpy(dmi.tSpec.target_type, dmTgtStr, sizeof(dmi.tSpec.target_type));
    dmi.tSpec.target_type[sizeof(dmi.tSpec.target_type) - 1] = '\0';
    char *dmTblEnd = stpncpy(dmi.dmTbl, rfsMeta->dmTableVerint, sizeof(dmi.dmTbl) - 1);
    *dmTblEnd = '\0';
    dmi.ioctl.data_size = dmTblEnd + 1 - (char *)&dmi.ioctl;
    if (cominitDmctlLoadDmTable(dmCtlFd, &dmi) == -1) {
        cominitErrnoPrint("Could not load device mapper table using ioctl().");
        close(dmCtlFd);
        return -1;
    }

    if (cominitDmctlStartDmDevice(dmCtlFd, &dmi, devId) == -1) {
        cominitErrnoPrint("Could not make the device mapper resume using ioctl().");
        close(dmCtlFd);
        return -1;
    }

    // Write new device path to metadata struct and create device node.
    strcpy(rfsMeta->devicePath, "/dev/" DM_DIR "/" COMINIT_ROOTFS_DM_NAME);
    mode_t devMode = S_IFBLK | S_IRUSR;
    if (!rfsMeta->ro) {
        devMode += S_IWUSR;
    }
    if (mknod(rfsMeta->devicePath, devMode, devId) == -1) {
        cominitErrnoPrint("Could not create device-mapper rootfs node at \'%s\'.", rfsMeta->devicePath);
        close(dmCtlFd);
        return -1;
    }

    close(dmCtlFd);

    return 0;
}

int cominitSetupDmDeviceCrypt(char *device, const char *name, const TPM2B_DIGEST *key, uint64_t offsetSectors) {
    if (access(device, F_OK) != 0) {
        cominitErrnoPrint("/'%s/' not ready", device);
        return -1;
    }

    if (cominitDmctlCheckForCryptoAlg("xts(aes)") != 0) {
        cominitErrnoPrint("Kernel does not support /'%s'/ crypt algorithm", "xts(aes)");
        return -1;
    }

    int dmCtlFd = open("/dev/" DM_DIR "/" DM_CONTROL_NODE, O_RDWR);
    if (dmCtlFd == -1) {
        cominitErrnoPrint("Could not open \'/dev/" DM_DIR "/" DM_CONTROL_NODE "\'.");
        return -1;
    }

    cominitDmIoctlData_t dmi;
    if (cominitDmctlCreateNewDmDevice(dmCtlFd, &dmi, name) == -1) {
        cominitErrnoPrint("Could not create new device mapper device using ioctl().");
        close(dmCtlFd);
        return -1;
    }

    // Load dm-verity table to device mapper.
    uint64_t devId = dmi.ioctl.dev;

    memset(&dmi, 0, sizeof(dmi));
    cominitIoctlSetVersion(dmi.ioctl);

    unsigned long totalSectors = 0;
    if (cominitDmctlGetSectors(device, &totalSectors) == -1) {
        cominitErrnoPrint("Could not calculate sectors of device '/%s/'.", device);
        close(dmCtlFd);
        return -1;
    }
    unsigned long mapLength = totalSectors - offsetSectors;

    size_t hexLen = key->size * 2 + 1;
    char *keyHex = malloc(hexLen);
    if (cominitBytesToHex(keyHex, key->buffer, key->size) == -1) {
        cominitErrnoPrint("Could not convert bytes to hex.", device);
        /* Overwrite key in memory */
        memset(keyHex, 0, hexLen);
        free(keyHex);
        close(dmCtlFd);
        return -1;
    }

    uint64_t start = 0;
    uint64_t ivOffset = 0;

    snprintf(dmi.dmTbl, sizeof(dmi.dmTbl), "aes-xts-plain64 %s %" PRIu64 " %s %" PRIu64 "", keyHex, ivOffset, device,
             offsetSectors);

    /* Overwrite key in memory */
    memset(keyHex, 0, hexLen);
    free(keyHex);

    strncpy(dmi.tSpec.target_type, "crypt", sizeof(dmi.tSpec.target_type));
    dmi.tSpec.target_type[sizeof(dmi.tSpec.target_type) - 1] = '\0';

    dmi.tSpec.sector_start = start;
    dmi.tSpec.length = mapLength;

    size_t tableLen = strlen(dmi.dmTbl) + 1;
    dmi.tSpec.next = sizeof(struct dm_target_spec) + tableLen;
    dmi.ioctl.dev = devId;
    dmi.ioctl.flags = 0;
    dmi.ioctl.target_count = 1;
    dmi.ioctl.data_start = offsetof(cominitDmIoctlData_t, tSpec) - offsetof(cominitDmIoctlData_t, ioctl);
    dmi.ioctl.data_size = dmi.ioctl.data_start + dmi.tSpec.next;
    if (cominitDmctlLoadDmTable(dmCtlFd, &dmi) == -1) {
        cominitErrnoPrint("Could not load device mapper table using ioctl().");
        close(dmCtlFd);
        return -1;
    }

    /* Overwrite key in memory */
    memset(&dmi.dmTbl, 0, sizeof(dmi.dmTbl));

    if (cominitDmctlStartDmDevice(dmCtlFd, &dmi, devId) == -1) {
        cominitErrnoPrint("Could not make the device mapper resume using ioctl().");
        close(dmCtlFd);
        return -1;
    }

    // create device node.
    if (mknod(COMINIT_TPM_SECURE_STORAGE_LOCATION, S_IFBLK | S_IRUSR | S_IWUSR, devId) == -1) {
        cominitErrnoPrint("Could not create device-mapper rootfs node at \'%s\'.", COMINIT_TPM_SECURE_STORAGE_LOCATION);
        close(dmCtlFd);
        return -1;
    }

    close(dmCtlFd);
    return 0;
}