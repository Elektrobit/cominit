// SPDX-License-Identifier: MIT
/**
 * @file mock_umount2.c
 * @brief Implementation of a mock function for umount2() using cmocka.
 */
#include "mock_umount2.h"

#include "unit_test.h"

int __wrap_umount2(const char *target, int flags) {  // NOLINT(readability-identifier-naming)
                                                     //     Rationale: Naming scheme fixed due to linker wrapping.
    check_expected_ptr(target);
    check_expected(flags);
    return mock_type(int);
}
