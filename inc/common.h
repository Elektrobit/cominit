// SPDX-License-Identifier: MIT
/**
 * @file common.h
 * @brief Header for common definitions not related to other specific features.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>
#include <tss2/tss2_esys.h>

#include "meta.h"
#include "output.h"

/**
 * Structure holding parsed options from argv.
 */
typedef struct cominitCliArgs {
    bool pcrSet;                               ///< Flag to check whether pcrIndex is set to a valid value.
    unsigned long pcrIndex;                    ///< The index of the SHA-256 bank of the TPM.
    int pcrSealCount;                          ///< The number of registers in the SHA-256 bank used for sealing.
    unsigned long pcrSeal[TPM2_PT_PCR_COUNT];  ///< The list of registers in the SHA-256 bank used for sealing.
    cominitLogLevelE_t visibleLogLevel;        ///< The visible log level.

    char devNodeRootFs[COMINIT_ROOTFS_DEV_PATH_MAX];  ///< Holds the Rootfs device node.
    char devNodeBlob[COMINIT_ROOTFS_DEV_PATH_MAX];    ///< Holds the blob device node.
    char devNodeCrypt[COMINIT_ROOTFS_DEV_PATH_MAX];   ///< Holds the crypt device node.
} cominitCliArgs_t;

/**
 * Suppress unused parameter warning for variable as per coding guideline.
 *
 * May be needed if an external interface is implemented.
 *
 * @param par  Unused variable that should not be warned about.
 */
#define COMINIT_PARAM_UNUSED(par) \
    do {                          \
        (void)(par);              \
    } while (0)

/**
 * Helper macro to count the number of elements in an array.
 *
 * @param array  The array that elements should be counted.
 */
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(array) (sizeof(array) / sizeof((array)[0]))
#endif

#endif /* __COMMON_H__ */
