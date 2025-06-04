// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_parse_public_keyfile.h
 * @brief Header declaring a mock function for mbedtls_pk_parse_public_keyfile().
 */
#ifndef __MOCK_MBEDTLS_PK_PARSE_PUBLIC_KEYFILE_H__
#define __MOCK_MBEDTLS_PK_PARSE_PUBLIC_KEYFILE_H__

#include <mbedtls/pk.h>

/**
 * Mock function for mbedtls_pk_parse_public_keyfile().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_pk_parse_public_keyfile(mbedtls_pk_context *ctx, const char *path);

#endif /* __MOCK_MBEDTLS_PK_PARSE_PUBLIC_KEYFILE_H__ */
