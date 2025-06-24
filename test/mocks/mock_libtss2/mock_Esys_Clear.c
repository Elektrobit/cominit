// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Clear.c
 * @brief Implementation of a mock function for Esys_Clear() using cmocka.
 */
#include "mock_Esys_Clear.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_Clear(
    ESYS_CONTEXT *esysContext,
    ESYS_TR authHandle,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3) {
    check_expected_ptr(esysContext);
    check_expected(authHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
}
