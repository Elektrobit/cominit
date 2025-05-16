// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_write_pubkey_der.h
 * @brief Header declaring a mock function for mbedtls_pk_write_pubkey_der().
 */
#ifndef __MOCK_MBEDTLS_PK_WRITE_PUBKEY_DER_H__
#define __MOCK_MBEDTLS_PK_WRITE_PUBKEY_DER_H__

#include <mbedtls/pk.h>

/**
 * Mock function for mbedtls_pk_write_pubkey_der().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mbedtls_pk_write_pubkey_der(mbedtls_pk_context *ctx, unsigned char *buf, size_t size);

#endif /* __MOCK_MBEDTLS_PK_WRITE_PUBKEY_DER_H__ */
