// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitKeyringGetKey.h
 * @brief Header declaring a mock function for cominitKeyringGetKey().
 */
#ifndef __MOCK_COMINIT_KEYRINGGETKEY_H__
#define __MOCK_COMINIT_KEYRINGGETKEY_H__

#include <stddef.h>
#include <stdint.h>

/**
 * Mock function for cominitKeyringGetKey().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitKeyringGetKey(uint8_t *key, size_t keyMaxLen, char *keyDesc);

#endif /* __MOCK_COMINIT_KEYRINGGETKEY_H__ */
