// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Initialize.c
 * @brief Implementation of a mock function for Esys_Initialize() using cmocka.
 */
#include "mock_Esys_Initialize.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_Initialize(ESYS_CONTEXT **esys_context,
                               TSS2_TCTI_CONTEXT *tcti,
                               TSS2_ABI_VERSION *abiVersion) {
    check_expected_ptr(esys_context);
    check_expected_ptr(tcti);
    check_expected_ptr(abiVersion);

    return TSS2_RC_SUCCESS;
}
