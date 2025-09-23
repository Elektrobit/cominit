// SPDX-License-Identifier: MIT
/**
 * @file mock_memcpy.c
 * @brief Implementation of a mock function for memcpy().
 */
#include "mock_memcpy.h"

#include "unit_test.h"

// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
int __wrap_memcpy(void *dest, const void *src, size_t n) {
    check_expected_ptr(dest);
    check_expected_ptr(src);
    check_expected(n);

    return mock_type(int);
}
