// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_TR_SetAuth.c
 * @brief Implementation of a mock function for Esys_TR_SetAuth() using cmocka.
 */
#include "mock_Esys_TR_SetAuth.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_TR_SetAuth(ESYS_CONTEXT *esysContext, ESYS_TR handle, TPM2B_AUTH const *authValue) {
    check_expected_ptr(esysContext);
    check_expected(handle);
    check_expected_ptr(authValue);
}
