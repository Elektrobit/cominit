// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_GetRandom.h
 * @brief Header declaring a mock function for Esys_GetRandom().
 */
#ifndef __MOCK_ESYS_GETRANDOM_H__
#define __MOCK_ESYS_GETRANDOM_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_GetRandom().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_GetRandom(ESYS_CONTEXT *esysContext, ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
                           UINT16 bytesRequested, TPM2B_DIGEST **randomBytes);

#endif /* __MOCK_ESYS_GETRANDOM_H__ */
