// SPDX-License-Identifier: MIT
/**
 * @file common.h
 * @brief Header for common definitions not related to other specific features.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdbool.h>

#include "meta.h"
/**
 * Structure holding parsed options from argv.
 */
typedef struct cominitCliArgs {
    bool pcrSet;                                      ///< Flag to check whether pcrIndex is set to a valid value.
    unsigned long pcrIndex;                           ///< The index of the SHA-256 bank of the TPM.
    char devNodeRootFs[COMINIT_ROOTFS_DEV_PATH_MAX];  ///< Holds the Rootfs device node.
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

#endif /* __COMMON_H__ */
