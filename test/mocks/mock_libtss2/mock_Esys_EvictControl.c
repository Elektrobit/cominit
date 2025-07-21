// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_EvictControl.c
 * @brief Implementation of a mock function for Esys_EvictControl() using cmocka.
 */
#include "mock_Esys_EvictControl.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_EvictControl(ESYS_CONTEXT *esysContext, ESYS_TR auth, ESYS_TR objectHandle, ESYS_TR shandle1,
                              ESYS_TR shandle2, ESYS_TR shandle3, TPMI_DH_PERSISTENT persistentHandle,
                              ESYS_TR *newObjectHandle) {
    check_expected_ptr(esysContext);
    check_expected(auth);
    check_expected_ptr(objectHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected_ptr(persistentHandle);
    check_expected_ptr(newObjectHandle);
}
