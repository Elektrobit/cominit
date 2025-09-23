// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-create-luks-volume-param-failure.c
 * @brief Implementation of a failure case unit test for cominitCryptsetupCreateLuksVolume().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>

#include "utest-cryptsetup-create-luks-volume.h"

void cominitCryptsetupCreateLuksVolumeTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    unsigned char passphrase[] = "secret key";
    size_t passphraseSize = ARRAY_SIZE(passphrase);

    assert_int_not_equal(cominitCryptsetupCreateLuksVolume(NULL, passphrase, passphraseSize), 0);
}
