// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-open-luks-volume-param-failure.c
 * @brief Implementation of a failure case unit test for cominitCryptsetupOpenLuksVolume().
 */

#include <cmocka_extensions/cmocka_extensions.h>

#include "utest-cryptsetup-open-luks-volume.h"

void cominitCryptsetupOpenLuksVolumeTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    assert_int_not_equal(cominitCryptsetupOpenLuksVolume(NULL), 0);
}
