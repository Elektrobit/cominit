// SPDX-License-Identifier: MIT
/**
 * @file common.c
 * @brief Implementation of common definitions not related to other specific features.
 */

#include "common.h"

#include <linux/fs.h>
#include <sys/ioctl.h>

int cominitCommonGetPartSize(uint64_t *partSize, int fd) {
    if (partSize == NULL) {
        cominitErrPrint("Return pointer must not be NULL.");
        return -1;
    }
    if (ioctl(fd, BLKGETSIZE64, partSize) == -1) {
        cominitErrnoPrint("Could not determine size of partition.");
        return -1;
    }
    return 0;
}