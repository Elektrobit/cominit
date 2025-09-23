// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitKeyringAddUserKey.h
 * @brief Header declaring a mock function for cominitKeyringAddUserKey().
 */
#ifndef __MOCK_COMINIT_SECUREMEMORYADDUSERKEY_H__
#define __MOCK_COMINIT_SECUREMEMORYADDUSERKEY_H__

#include <stddef.h>
#include <stdint.h>

/**
 * Mock function for cominitKeyringAddUserKey().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitKeyringAddUserKey(const char *keyDesc, const uint8_t *key, size_t keyLen);

#endif /* __MOCK_COMINIT_SECUREMEMORYADDUSERKEY_H__ */
