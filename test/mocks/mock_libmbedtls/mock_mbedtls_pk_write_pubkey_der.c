// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_write_pubkey_der.c
 * @brief Implementation of a mock function for mbedtls_pk_write_pubkey_der() using cmocka.
 */
#include "mock_mbedtls_pk_write_pubkey_der.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_pk_write_pubkey_der(mbedtls_pk_context *ctx, unsigned char *buf, size_t size) {
    assert_non_null(ctx);
    assert_non_null(buf);
    assert_true(size > 0);

    return 1;
}
