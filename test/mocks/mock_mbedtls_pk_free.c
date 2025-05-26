// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_free.c
 * @brief Implementation of a mock function for mbedtls_pk_free() using cmocka.
 */
#include "mock_mbedtls_pk_free.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_mbedtls_pk_free(mbedtls_pk_context *ctx) {
    assert_non_null(ctx);
}
