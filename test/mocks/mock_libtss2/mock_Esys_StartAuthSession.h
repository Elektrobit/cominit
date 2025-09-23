// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_StartAuthSession.h
 * @brief Header declaring a mock function for Esys_StartAuthSession().
 */
#ifndef __MOCK_ESYS_STARTAUTHSESSION_H__
#define __MOCK_ESYS_STARTAUTHSESSION_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_StartAuthSession().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_StartAuthSession(ESYS_CONTEXT *esysContext, ESYS_TR tpmKey, ESYS_TR bind, ESYS_TR shandle1,
                                     ESYS_TR shandle2, ESYS_TR shandle3, const TPM2B_NONCE *nonceCaller,
                                     TPM2_SE sessionType, const TPMT_SYM_DEF *symmetric, TPMI_ALG_HASH authHash,
                                     ESYS_TR *sessionHandle);

#endif /* __MOCK_ESYS_STARTAUTHSESSION_H__ */
