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

    struct cominitDmIoctlData_t {
        struct dm_ioctl ioctl;
        struct dm_target_spec tSpec;
        char dmTbl[COMINIT_DM_TABLE_SIZE_MAX];
    } dmi;

    int dmCtlFd = open("/dev/" DM_DIR "/" DM_CONTROL_NODE, O_RDWR);
    if (dmCtlFd == -1) {
        cominitErrnoPrint("Could not open \'/dev" DM_DIR "/" DM_CONTROL_NODE "\'.");
        return -1;
    }

    // Create new dm device.
    memset(&dmi.ioctl, 0, sizeof(dmi.ioctl));
    cominitIoctlSetVersion(dmi.ioctl);
    dmi.ioctl.data_size = sizeof(dmi.ioctl);
    strcpy(dmi.ioctl.name, COMINIT_ROOTFS_DM_NAME);
    if (ioctl(dmCtlFd, (int)DM_DEV_CREATE, &dmi.ioctl) == -1) {
        cominitErrnoPrint("Could not create new device mapper device using ioctl().");
        return -1;
    }
    uint64_t devId = dmi.ioctl.dev;

    // Load dm-verity table to device mapper.
    memset(&dmi, 0, sizeof(dmi));
    cominitIoctlSetVersion(dmi.ioctl);
    dmi.ioctl.dev = devId;
    dmi.ioctl.flags = rfsMeta->ro ? DM_READONLY_FLAG : 0;
    dmi.ioctl.target_count = 1;
    dmi.ioctl.data_start = offsetof(struct cominitDmIoctlData_t, tSpec) - offsetof(struct cominitDmIoctlData_t, ioctl);
    dmi.tSpec.sector_start = 0;
    dmi.tSpec.length = rfsMeta->dmVerintDataSizeBytes / 512;
    strncpy(dmi.tSpec.target_type, dmTgtStr, sizeof(dmi.tSpec.target_type));
    dmi.tSpec.target_type[sizeof(dmi.tSpec.target_type) - 1] = '\0';
    char *dmTblEnd = stpncpy(dmi.dmTbl, rfsMeta->dmTableVerint, sizeof(dmi.dmTbl) - 1);
    *dmTblEnd = '\0';
    dmi.ioctl.data_size = dmTblEnd + 1 - (char *)&dmi.ioctl;
    if (ioctl(dmCtlFd, (int)DM_TABLE_LOAD, &dmi.ioctl) == -1) {
        cominitErrnoPrint("Could not load device mapper table using ioctl().");
        return -1;
    }

    // Let device mapper resume.
    memset(&dmi.ioctl, 0, sizeof(dmi.ioctl));
    cominitIoctlSetVersion(dmi.ioctl);
    dmi.ioctl.data_size = sizeof(dmi.ioctl);
    dmi.ioctl.dev = devId;
    // DM_SUSPEND_FLAG is unset, so DM_DEV_SUSPEND will resume.
    if (ioctl(dmCtlFd, (int)DM_DEV_SUSPEND, &dmi.ioctl) == -1) {
        cominitErrnoPrint("Could not make the device mapper resume using ioctl().");
        return -1;
    }

    // Write new device path to metadata struct and create device node.
    strcpy(rfsMeta->devicePath, "/dev/" DM_DIR "/" COMINIT_ROOTFS_DM_NAME);
    mode_t devMode = S_IFBLK | 0400;
    if (!rfsMeta->ro) {
        devMode += 0200;
    }
    if (mknod(rfsMeta->devicePath, devMode, devId) == -1) {
        cominitErrnoPrint("Could not create device-mapper rootfs node at \'%s\'.", rfsMeta->devicePath);
        return -1;
    }

    return 0;
}

int cominitSetupDmDeviceCrypt(char *device, const char *name, const TPM2B_DIGEST *key, uint64_t offsetSectors) {
    struct cominitDmIoctlData_t {
        struct dm_ioctl ioctl;
        struct dm_target_spec tSpec;
        char dmTbl[COMINIT_DM_TABLE_SIZE_MAX];
    } dmi;

    int dmCtlFd = open("/dev/" DM_DIR "/" DM_CONTROL_NODE, O_RDWR);
    if (dmCtlFd == -1) {
        cominitErrnoPrint("Could not open \'/dev/" DM_DIR "/" DM_CONTROL_NODE "\'.");
        return -1;
    }

    // Create new dm device.
    memset(&dmi.ioctl, 0, sizeof(dmi.ioctl));
    cominitIoctlSetVersion(dmi.ioctl);
    dmi.ioctl.data_size = sizeof(dmi.ioctl);
    strcpy(dmi.ioctl.name, name);
    if (ioctl(dmCtlFd, (int)DM_DEV_CREATE, &dmi.ioctl) == -1) {
        cominitErrnoPrint("Could not create new device mapper device using ioctl().");
        return -1;
    }

    if (access(device, F_OK) != 0) {
        cominitErrnoPrint("/'%s/' not ready", device);
        return -1;
    }

    if (cominitDmctlCheckForCryptoAlg("xts(aes)") != 0) {
        cominitErrnoPrint("Kernel does not support /'%s'/ crypt algorithm", "xts(aes)");
    }

    // Load dm-verity table to device mapper.
    uint64_t devId = dmi.ioctl.dev;

    memset(&dmi, 0, sizeof(dmi));
    cominitIoctlSetVersion(dmi.ioctl);

    unsigned long totalSectors = 0;
    if (cominitDmctlGetSectors(device, &totalSectors) == -1) {
        cominitErrnoPrint("Could not calculate sectors of device '/%s/'.", device);
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
    dmi.ioctl.data_start = offsetof(struct cominitDmIoctlData_t, tSpec) - offsetof(struct cominitDmIoctlData_t, ioctl);
    dmi.ioctl.data_size = dmi.ioctl.data_start + dmi.tSpec.next;

    if (ioctl(dmCtlFd, (int)DM_TABLE_LOAD, &dmi.ioctl) == -1) {
        cominitErrnoPrint("Could not load device mapper table using ioctl().");
        return -1;
    }

    /* Overwrite key in memory */
    memset(&dmi.dmTbl, 0, sizeof(dmi.dmTbl));

    // Let device mapper resume.
    memset(&dmi.ioctl, 0, sizeof(dmi.ioctl));
    cominitIoctlSetVersion(dmi.ioctl);
    dmi.ioctl.data_size = sizeof(dmi.ioctl);
    dmi.ioctl.dev = devId;
    // DM_SUSPEND_FLAG is unset, so DM_DEV_SUSPEND will resume.
    if (ioctl(dmCtlFd, (int)DM_DEV_SUSPEND, &dmi.ioctl) == -1) {
        cominitErrnoPrint("Could not make the device mapper resume using ioctl().");
        return -1;
    }

    // create device node.
    mode_t devMode = S_IFBLK | 0400 | 0200;

    if (mknod(COMINIT_TPM_SECURE_STORAGE_LOCATION, devMode, devId) == -1) {
        cominitErrnoPrint("Could not create device-mapper rootfs node at \'%s\'.", COMINIT_TPM_SECURE_STORAGE_LOCATION);
        return -1;
    }

    close(dmCtlFd);
    return 0;
}