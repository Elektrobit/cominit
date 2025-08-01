// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Free.c
 * @brief Implementation of a mock function for Esys_Free() using cmocka.
 */
#include "mock_Esys_Free.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_Free(void *__ptr) {
    check_expected_ptr(__ptr);
}
