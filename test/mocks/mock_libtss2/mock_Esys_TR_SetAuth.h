// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_TR_SetAuth.h
 * @brief Header declaring a mock function for Esys_TR_SetAuth().
 */
#ifndef __MOCK_ESYS_TR_SETAUTH_H__
#define __MOCK_ESYS_TR_SETAUTH_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_TR_SetAuth().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_TR_SetAuth(
    ESYS_CONTEXT *esysContext,
    ESYS_TR handle,
    TPM2B_AUTH const *authValue);

#endif /* __MOCK_ESYS_TR_SETAUTH_H__ */
