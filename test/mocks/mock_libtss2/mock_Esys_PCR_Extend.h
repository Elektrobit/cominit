// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_PCR_Extend.h
 * @brief Header declaring a mock function for Esys_PCR_Extend().
 */
#ifndef __MOCK_ESYS_PCR_EXTEND_H__
#define __MOCK_ESYS_PCR_EXTEND_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_PCR_Extend().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_PCR_Extend(ESYS_CONTEXT *esysContext, ESYS_TR pcrHandle, ESYS_TR shandle1, ESYS_TR shandle2,
                               ESYS_TR shandle3, const TPML_DIGEST_VALUES *digests);

#endif /* __MOCK_ESYS_PCR_EXTEND_H__ */
