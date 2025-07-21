// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_FlushContext.h
 * @brief Header declaring a mock function for Esys_FlushContext().
 */
#ifndef __MOCK_ESYS_FLUSHCONTEXT_H__
#define __MOCK_ESYS_FLUSHCONTEXT_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_FlushContext().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_FlushContext(ESYS_CONTEXT *esysContext, ESYS_TR flushHandle);

#endif /* __MOCK_ESYS_FLUSHCONTEXT_H__ */
