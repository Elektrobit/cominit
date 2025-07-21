// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Initialize.h
 * @brief Header declaring a mock function for Esys_Initialize().
 */
#ifndef __MOCK_ESYS_INITIALIZE_H__
#define __MOCK_ESYS_INITIALIZE_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_Initialize().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_Initialize(ESYS_CONTEXT **esys_context, TSS2_TCTI_CONTEXT *tcti, TSS2_ABI_VERSION *abiVersion);

#endif /* __MOCK_ESYS_INITIALIZE_H__ */
