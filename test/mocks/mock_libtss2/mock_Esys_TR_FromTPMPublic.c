// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_TR_FromTPMPublic.c
 * @brief Implementation of a mock function for Esys_TR_FromTPMPublic() using cmocka.
 */
#include "mock_Esys_TR_FromTPMPublic.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_TR_FromTPMPublic(ESYS_CONTEXT *esys_context, TPM2_HANDLE tpm_handle, ESYS_TR shandle1,
                                     ESYS_TR shandle2, ESYS_TR shandle3, ESYS_TR *object) {
    check_expected_ptr(esys_context);
    check_expected(tpm_handle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);

    assert_non_null(object);

    return mock_type(TSS2_RC);
}
