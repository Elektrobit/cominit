// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Unseal.h
 * @brief Header declaring a mock function for Esys_Unseal().
 */
#ifndef __MOCK_ESYS_UNSEAL_H__
#define __MOCK_ESYS_UNSEAL_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_Unseal().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_Unseal(
    ESYS_CONTEXT *esysContext,
    ESYS_TR itemHandle,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    TPM2B_SENSITIVE_DATA **outData);

#endif /* __MOCK_ESYS_UNSEAL_H__ */
