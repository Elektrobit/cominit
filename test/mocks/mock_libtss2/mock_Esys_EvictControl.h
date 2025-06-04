// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_EvictControl.h
 * @brief Header declaring a mock function for Esys_EvictControl().
 */
#ifndef __MOCK_ESYS_EVICTCONTROL_H__
#define __MOCK_ESYS_EVICTCONTROL_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_EvictControl().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_EvictControl(
    ESYS_CONTEXT *esysContext,
    ESYS_TR auth,
    ESYS_TR objectHandle,
    ESYS_TR shandle1,
    ESYS_TR shandle2,
    ESYS_TR shandle3,
    TPMI_DH_PERSISTENT persistentHandle,
    ESYS_TR *newObjectHandle);

#endif /* __MOCK_ESYS_EVICTCONTROL_H__ */
