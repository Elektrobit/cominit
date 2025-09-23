// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-passphrase-failure.c
 * @brief Implementation of a failure case unit test for cominitCryptoCreatePassphrase().
 */
#include <stdlib.h>

#include "unit_test.h"
#include "utest-crypto-create-passphrase.h"

void cominitCryptoCreatePassphraseTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    unsigned char passphrase[] = "";
    size_t passphraseSize = sizeof passphrase;

    assert_int_not_equal(cominitCryptoCreatePassphrase(NULL, passphraseSize), 0);

    assert_int_not_equal(cominitCryptoCreatePassphrase(passphrase, 0), 0);
}
