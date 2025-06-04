// SPDX-License-Identifier: MIT
/**
 * @file mock_mbedtls_pk_free.h
 * @brief Header declaring a mock function for mbedtls_pk_free().
 */
#ifndef __MOCK_MBEDTLS_PK_FREE_H__
#define __MOCK_MBEDTLS_PK_FREE_H__

#include <mbedtls/pk.h>

/**
 * Mock function for mbedtls_pk_free().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_mbedtls_pk_free(mbedtls_pk_context *ctx);

#endif /* __MOCK_MBEDTLS_PK_FREE_H__ */
