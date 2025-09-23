// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_sha256_ret.h
 * @brief Header declaring a mock function for mbedtls_sha256_ret().
 */
#ifndef __MOCK_MBEDTLS_SHA256_RET_H__
#define __MOCK_MBEDTLS_SHA256_RET_H__

#include <stddef.h>

/**
 * Mock function for mbedtls_sha256_ret().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_sha256_ret(const unsigned char *input, size_t ilen, unsigned char output[32], int is224);

#endif /* __MOCK_MBEDTLS_SHA256_RET_H__ */
