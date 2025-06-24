// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_SelfTest.h
 * @brief Header declaring a mock function for Esys_SelfTest().
 */
#ifndef __MOCK_ESYS_SELFTEST_H__
#define __MOCK_ESYS_SELFTEST_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_SelfTest().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_SelfTest(ESYS_CONTEXT *esysContext,
                             ESYS_TR shandle1,
                             ESYS_TR shandle2,
                             ESYS_TR shandle3,
                             TPMI_YES_NO fullTest);

#endif /* __MOCK_ESYS_SELFTEST_H__ */
