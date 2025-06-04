// SPDX-License-Identifier: MIT
/**
 * @file mock_Tss2_TctiLdr_Initialize.h
 * @brief Header declaring a mock function for Tss2_TctiLdr_Initialize().
 */
#ifndef __MOCK_TSS2_TCTILDR_INITIALIZE_H__
#define __MOCK_TSS2_TCTILDR_INITIALIZE_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_tcti.h>

/**
 * Mock function for Tss2_TctiLdr_Initialize().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Tss2_TctiLdr_Initialize(const char *nameConf,
                                       TSS2_TCTI_CONTEXT **context);

#endif /* __MOCK_TSS2_TCTILDR_INITIALIZE_H__ */
