// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptoCreatePassphrase.h
 * @brief Header declaring a mock function for cominitCryptoCreatePassphrase().
 */
#ifndef __MOCK_COMINIT_CRYPTOCREATEPASSPHRASE_H__
#define __MOCK_COMINIT_CRYPTOCREATEPASSPHRASE_H__

#include <stddef.h>

/**
 * Mock function for cominitCryptoCreatePassphrase().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptoCreatePassphrase(unsigned char *passphrase, size_t passphraseSize);

#endif /* __MOCK_COMINIT_CRYPTOCREATEPASSPHRASE_H__ */
