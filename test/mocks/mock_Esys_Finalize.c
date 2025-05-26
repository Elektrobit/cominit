// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Finalize.c
 * @brief Implementation of a mock function for Esys_Finalize() using cmocka.
 */
#include "mock_Esys_Finalize.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_Finalize(ESYS_CONTEXT **context) {
    assert_non_null(context);
}
