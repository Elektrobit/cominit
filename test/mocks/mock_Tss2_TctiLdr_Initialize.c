// SPDX-License-Identifier: MIT
/**
 * @file mock_Tss2_TctiLdr_Initialize.c
 * @brief Implementation of a mock function for Tss2_TctiLdr_Initialize() using cmocka.
 */
#include "mock_Tss2_TctiLdr_Initialize.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Tss2_TctiLdr_Initialize(const char *nameConf, TSS2_TCTI_CONTEXT **context) {
    check_expected_ptr(nameConf);
    check_expected_ptr(context);

    return TSS2_RC_SUCCESS;
}
