// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_FlushContext.c
 * @brief Implementation of a mock function for Esys_FlushContext() using cmocka.
 */
#include "mock_Esys_FlushContext.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_FlushContext(ESYS_CONTEXT *esysContext, ESYS_TR flushHandle) {
    check_expected_ptr(esysContext);
    check_expected(flushHandle);

    return mock_type(TSS2_RC);
}
