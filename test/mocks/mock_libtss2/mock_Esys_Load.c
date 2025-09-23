// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Load.c
 * @brief Implementation of a mock function for Esys_Load() using cmocka.
 */
#include "mock_Esys_Load.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_Load(ESYS_CONTEXT *esysContext, ESYS_TR parentHandle, ESYS_TR shandle1, ESYS_TR shandle2,
                         ESYS_TR shandle3, const TPM2B_PRIVATE *inPrivate, const TPM2B_PUBLIC *inPublic,
                         ESYS_TR *objectHandle) {
    check_expected_ptr(esysContext);
    check_expected(parentHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected(inPrivate);
    check_expected(inPublic);

    assert_non_null(objectHandle);

    return mock_type(TSS2_RC);
}
