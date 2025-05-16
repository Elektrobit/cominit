// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_SelfTest.c
 * @brief Implementation of a mock function for Esys_SelfTest() using cmocka.
 */
#include "mock_Esys_SelfTest.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_SelfTest(ESYS_CONTEXT *esysContext, ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
                             TPMI_YES_NO fullTest) {
    check_expected_ptr(esysContext);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected(fullTest);

    return TSS2_RC_SUCCESS;
}
