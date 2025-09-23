// SPDX-License-Identifier: MIT
/**
 * @file mock_lstat.c
 * @brief Implementation of a mock function for lstat().
 */
#include "mock_lstat.h"

#include "unit_test.h"

// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
int __wrap_lstat(const char *restrict path, struct stat *restrict buf) {
    check_expected_ptr(path);
    check_expected_ptr(buf);

    return mock_type(int);
}
