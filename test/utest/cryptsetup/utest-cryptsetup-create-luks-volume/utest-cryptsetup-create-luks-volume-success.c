// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-create-luks-volume-success.c
 * @brief Implementation of a success case unit test for cominitCryptsetupCreateLuksVolume().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>

#include "utest-cryptsetup-create-luks-volume.h"

void cominitCryptsetupCreateLuksVolumeTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char devCryptTest[] = "/dev/crypt";

    unsigned char passphraseTest[] = "secret key";
    size_t passphraseSizeTest = ARRAY_SIZE(passphraseTest);

    expect_string(__wrap_cominitSubprocessSpawnAndWrite, data, passphraseTest);
    expect_value(__wrap_cominitSubprocessSpawnAndWrite, dataSize, passphraseSizeTest);
    expect_string(__wrap_cominitSubprocessSpawnAndWrite, path, COMINIT_CRYPTSETUP_DIR);
    expect_any(__wrap_cominitSubprocessSpawnAndWrite, argv);
    expect_any(__wrap_cominitSubprocessSpawnAndWrite, env);

    will_return(__wrap_cominitSubprocessSpawnAndWrite, 0);

    assert_int_equal(cominitCryptsetupCreateLuksVolume(devCryptTest, passphraseTest, passphraseSizeTest), 0);
}
