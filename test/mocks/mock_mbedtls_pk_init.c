// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_init.c
 * @brief Implementation of a mock function for mbedtls_pk_init() using cmocka.
 */
#include "mock_mbedtls_pk_init.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_mbedtls_pk_init(mbedtls_pk_context *ctx) {
    assert_non_null(ctx);
}
