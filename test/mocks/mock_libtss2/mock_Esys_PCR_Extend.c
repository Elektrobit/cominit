// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_PCR_Extend.c
 * @brief Implementation of a mock function for Esys_PCR_Extend() using cmocka.
 */
#include "mock_Esys_PCR_Extend.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_PCR_Extend(ESYS_CONTEXT *esysContext, ESYS_TR pcrHandle, ESYS_TR shandle1, ESYS_TR shandle2,
                               ESYS_TR shandle3, const TPML_DIGEST_VALUES *digests) {
    check_expected_ptr(esysContext);
    check_expected(pcrHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);

    assert_non_null(digests);

    return mock_type(TSS2_RC);
}
