// SPDX-License-Identifier: MIT
/**
 * @file common.h
 * @brief Header for common definitions not related to other specific features.
 */
#ifndef __COMMON_H__
#define __COMMON_H__

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
