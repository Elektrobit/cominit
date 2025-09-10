// SPDX-License-Identifier: MIT
/**
 * @file mock_malloc.c
 * @brief Implementation of a mock function for malloc().
 */
#include "mock_malloc.h"

#include "unit_test.h"

bool cominitMockMallocEnabled = false;

// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
void *__wrap_malloc(size_t size) {
    if (cominitMockMallocEnabled) {
        check_expected(size);
        return mock_ptr_type(void *);
    } else {
        return __real_malloc(size);
    }
}
