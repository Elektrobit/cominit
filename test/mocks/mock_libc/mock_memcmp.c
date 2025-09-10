// SPDX-License-Identifier: MIT
/**
 * @file mock_memcmp.c
 * @brief Implementation of a mock function for memcmp().
 */
#include "mock_memcmp.h"

#include "unit_test.h"

// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
int __wrap_memcmp(const void *s1, const void *s2, size_t n) {
    check_expected_ptr(s1);
    check_expected_ptr(s2);
    check_expected(n);

    return mock_type(int);
}
