// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Finalize.h
 * @brief Header declaring a mock function for Esys_Finalize().
 */
#ifndef __MOCK_ESYS_FINALIZE_H__
#define __MOCK_ESYS_FINALIZE_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_Finalize().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_Finalize(ESYS_CONTEXT **context);

#endif /* __MOCK_ESYS_FINALIZE_H__ */
