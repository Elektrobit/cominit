// SPDX-License-Identifier: MIT
/**
 * @file mock_Tss2_TctiLdr_Finalize.c
 * @brief Implementation of a mock function for Tss2_TctiLdr_Finalize() using cmocka.
 */
#include "mock_Tss2_TctiLdr_Finalize.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Tss2_TctiLdr_Finalize(TSS2_TCTI_CONTEXT **context) {
    check_expected_ptr(context);
}
