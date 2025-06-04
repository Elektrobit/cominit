// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_StartAuthSession.c
 * @brief Implementation of a mock function for Esys_StartAuthSession() using cmocka.
 */
#include "mock_Esys_StartAuthSession.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_StartAuthSession(
    ESYS_CONTEXT *esysContext,
    ESYS_TR tpmKey,
    ESYS_TR bind,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    const TPM2B_NONCE *nonceCaller,
    TPM2_SE sessionType,
    const TPMT_SYM_DEF *symmetric,
    TPMI_ALG_HASH authHash,
    ESYS_TR *sessionHandle) {
    check_expected_ptr(esysContext);
    check_expected(tpmKey);
    check_expected(bind);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected_ptr(nonceCaller);
    check_expected(sessionType);
    check_expected_ptr(symmetric);
    check_expected(authHash);
    check_expected_ptr(sessionHandle);

    return mock_type(TSS2_RC);
}
