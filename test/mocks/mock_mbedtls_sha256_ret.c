// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_sha256_ret.c
 * @brief Implementation of a mock function for mbedtls_sha256_ret() using cmocka.
 */
#include "mock_mbedtls_sha256_ret.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_sha256_ret(const unsigned char *input, size_t ilen, unsigned char output[32], int is224) {
    check_expected_ptr(input);
    check_expected(ilen);
    check_expected_ptr(output);
    check_expected(is224);

    return 0;
}
