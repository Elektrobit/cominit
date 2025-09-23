// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Unseal.c
 * @brief Implementation of a mock function for Esys_Unseal() using cmocka.
 */
#include "mock_Esys_Unseal.h"

#include <errno.h>

#include "unit_test.h"

__attribute__((weak))
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC
__wrap_Esys_Unseal(ESYS_CONTEXT *esysContext, ESYS_TR itemHandle, ESYS_TR shandle1, ESYS_TR shandle2, ESYS_TR shandle3,
                   TPM2B_SENSITIVE_DATA **outData) {
    check_expected_ptr(esysContext);
    check_expected(itemHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected_ptr(outData);

    return mock_type(TSS2_RC);
}
