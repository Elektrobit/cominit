// SPDX-License-Identifier: MIT
/**
 * @file mock_mkdir.c
 * @brief Implementation of a mock function for mkdir() using cmocka.
 */
#include "mock_mkdir.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mkdir(const char *pathName, mode_t mode) {
    check_expected_ptr(pathName);
    check_expected(mode);
    errno = mock_type(int);
    return mock_type(int);
}
