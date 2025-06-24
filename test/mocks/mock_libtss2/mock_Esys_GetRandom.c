// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_GetRandom.c
 * @brief Implementation of a mock function for Esys_GetRandom() using cmocka.
 */
#include "mock_Esys_GetRandom.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_GetRandom(
    ESYS_CONTEXT *esysContext,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    UINT16 bytesRequested,
    TPM2B_DIGEST **randomBytes) {
    check_expected_ptr(esysContext);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected(bytesRequested);
    assert_non_null(randomBytes);
}
