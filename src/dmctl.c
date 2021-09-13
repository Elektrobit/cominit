// SPDX-License-Identifier: MIT
/**
 * @file dmctl.c
 * @brief Implementation of device-mapper setup.
 */
#include "dmctl.h"

#include <errno.h>
#include <fcntl.h>
#include <linux/dm-ioctl.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>

#include "output.h"

#define cominitIoctlSetVersion(ioctlStruct)               \
    do {                                                  \
        (ioctlStruct).version[0] = DM_VERSION_MAJOR;      \
        (ioctlStruct).version[1] = DM_VERSION_MINOR;      \
        (ioctlStruct).version[2] = DM_VERSION_PATCHLEVEL; \
    } while (0)

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
