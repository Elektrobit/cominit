// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-open-luks-volume-success.c
 * @brief Implementation of a success case unit test for cominitCryptsetupOpenLuksVolume().
 */

#include <cmocka_extensions/cmocka_extensions.h>

#include "utest-cryptsetup-open-luks-volume.h"

void cominitCryptsetupOpenLuksVolumeTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char devCryptTest[] = "/dev/crypt";

    expect_string(__wrap_cominitSubprocessSpawn, path, COMINIT_CRYPTSETUP_DIR);
    expect_any(__wrap_cominitSubprocessSpawn, argv);
    expect_any(__wrap_cominitSubprocessSpawn, env);

    will_return(__wrap_cominitSubprocessSpawn, 0);

    assert_int_equal(cominitCryptsetupOpenLuksVolume(devCryptTest), 0);
}
