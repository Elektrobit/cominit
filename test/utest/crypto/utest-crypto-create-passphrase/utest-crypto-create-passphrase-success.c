// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-passphrase-success.c
 * @brief Implementation of a success case unit test for cominitCryptoCreatePassphrase().
 */

#include <string.h>

#include "unit_test.h"
#include "utest-crypto-create-passphrase.h"

#define TEST_PHRASE "secret key"

void cominitCryptoCreatePassphraseTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    unsigned char passphrase[] = TEST_PHRASE;
    size_t passphraseSize = sizeof passphrase;
    unsigned char zero[passphraseSize];
    memset(zero, 0, passphraseSize);

    assert_int_equal(cominitCryptoCreatePassphrase(passphrase, passphraseSize), 0);

    /* Assert the passphrase was overwritten: it differs from TEST_PHRASE and contains at least one non-zero byte. */
    assert_string_not_equal(TEST_PHRASE, (char *)passphrase);
    assert_int_not_equal(memcmp(passphrase, zero, passphraseSize), 0);
}
