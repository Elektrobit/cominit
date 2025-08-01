// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-passphrase.h
 * @brief Header declaring cmocka unit test functions for cominitCryptoCreatePassphrase().
 */
#ifndef __UTEST_CRYPTO_CREATE_PASSPHRASE_H__
#define __UTEST_CRYPTO_CREATE_PASSPHRASE_H__

#include "common.h"
#include "crypto.h"

/**
 * Unit test for cominitCryptoCreatePassphrase() successful code path.
 * @param state
 */
void cominitCryptoCreatePassphraseTestSuccess(void **state);

/**
 * Unit test for cominitCryptoCreatePassphrase() with parameters not initialized.
 * @param state
 */
void cominitCryptoCreatePassphraseTestParamFailure(void **state);

#endif /* __UTEST_CRYPTO_CREATE_PASSPHRASE_H__ */
