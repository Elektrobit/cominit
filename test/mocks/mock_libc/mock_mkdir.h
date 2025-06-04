// SPDX-License-Identifier: MIT
/**
 * @file mock_mkdir.h
 * @brief Header declaring a mock function for mkdir().
 */
#ifndef __MOCK_MKDIR_H__
#define __MOCK_MKDIR_H__

#include <sys/stat.h>

/**
 * Mock function for mkdir().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mkdir(const char *pathName, mode_t mode);

#endif /* __MOCK_MKDIR_H__ */
