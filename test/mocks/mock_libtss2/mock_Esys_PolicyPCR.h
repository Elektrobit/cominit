// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_PolicyPCR.h
 * @brief Header declaring a mock function for Esys_PolicyPCR().
 */
#ifndef __MOCK_ESYS_POLICYPCR_H__
#define __MOCK_ESYS_POLICYPCR_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_PolicyPCR().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_PolicyPCR(
    ESYS_CONTEXT *esysContext,
    ESYS_TR policySession,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    const TPM2B_DIGEST *pcrDigest,
    const TPML_PCR_SELECTION *pcrs);

#endif /* __MOCK_ESYS_POLICYPCR_H__ */
