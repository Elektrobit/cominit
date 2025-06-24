// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_PolicyGetDigest.c
 * @brief Implementation of a mock function for Esys_PolicyGetDigest() using cmocka.
 */
#include "mock_Esys_PolicyGetDigest.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_PolicyGetDigest(
    ESYS_CONTEXT *esysContext,
    ESYS_TR policySession,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    TPM2B_DIGEST **policyDigest) {
    check_expected_ptr(esysContext);
    check_expected(policySession);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected_ptr(policyDigest);

    return mock_type(TSS2_RC);
}
