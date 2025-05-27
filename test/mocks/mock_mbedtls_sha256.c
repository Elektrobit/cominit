// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_sha256.c
 * @brief Implementation of a mock function for mbedtls_sha256() using cmocka.
 */
#include "mock_mbedtls_sha256.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_sha256(const unsigned char *input, size_t ilen, unsigned char output[32], int is224) {
    assert_non_null(input);
    assert_true(ilen > 0);
    assert_non_null(output);
    assert_true(is224 == 0);

    return 0;
}
