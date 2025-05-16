// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_parse_public_keyfile.c
 * @brief Implementation of a mock function for mbedtls_pk_parse_public_keyfile() using cmocka.
 */
#include "mock_mbedtls_pk_parse_public_keyfile.h"

#include <errno.h>

#include "unit_test.h"

    // NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_pk_parse_public_keyfile(mbedtls_pk_context *ctx, const char *path) {
    check_expected_ptr(ctx);
    check_expected_ptr(path);

    return 0;
}
