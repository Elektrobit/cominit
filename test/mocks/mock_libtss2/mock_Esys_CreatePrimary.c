// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_CreatePrimary.c
 * @brief Implementation of a mock function for Esys_CreatePrimary() using cmocka.
 */
#include "mock_Esys_CreatePrimary.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_CreatePrimary(
    ESYS_CONTEXT *esysContext,
    ESYS_TR primaryHandle,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    const TPM2B_SENSITIVE_CREATE *inSensitive,
    const TPM2B_PUBLIC *inPublic,
    const TPM2B_DATA *outsideInfo,
    const TPML_PCR_SELECTION *creationPCR,
    ESYS_TR *objectHandle,
    TPM2B_PUBLIC **outPublic,
    TPM2B_CREATION_DATA **creationData,
    TPM2B_DIGEST **creationHash,
    TPMT_TK_CREATION **creationTicket) {
    check_expected_ptr(esysContext);
    check_expected(primaryHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected_ptr(inSensitive);
    check_expected_ptr(inPublic);
    check_expected_ptr(outsideInfo);
    check_expected_ptr(creationPCR);
    check_expected_ptr(objectHandle);
    check_expected_ptr(outPublic);
    check_expected_ptr(creationData);
    check_expected_ptr(creationHash);
    check_expected_ptr(creationTicket);

    return mock_type(TSS2_RC);
}
