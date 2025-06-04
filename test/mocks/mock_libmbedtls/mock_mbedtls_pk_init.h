// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_init.h
 * @brief Header declaring a mock function for mbedtls_pk_init().
 */
#ifndef __MOCK_MBEDTLS_PK_INIT_H__
#define __MOCK_MBEDTLS_PK_INIT_H__

#include <mbedtls/pk.h>

/**
 * Mock function for mbedtls_pk_init().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_mbedtls_pk_init(mbedtls_pk_context *ctx);

#endif /* __MOCK_MBEDTLS_PK_INIT_H__ */
