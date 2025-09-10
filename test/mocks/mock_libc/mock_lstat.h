// SPDX-License-Identifier: MIT
/**
 * @file mock_lstat.h
 * @brief Header declaring a mock function for lstat().
 */
#ifndef __MOCK_LSTAT_H__
#define __MOCK_LSTAT_H__

#include <sys/stat.h>

/**
 * Mock function for lstat().
 *
 * Checks that the right parameters are given and return a preset pointer.
 */
int __wrap_lstat(const char *restrict path,
                 struct stat *restrict buf);  // NOLINT(readability-identifier-naming)
                                              // Rationale: Naming scheme fixed due to linker wrapping.

#endif /* __MOCK_LSTAT_H__ */
