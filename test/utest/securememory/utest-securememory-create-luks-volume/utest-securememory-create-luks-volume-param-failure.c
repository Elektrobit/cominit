// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-create-luks-volume-param-failure.c
 * @brief Implementation of a failure case unit test for cominitSecurememoryCreateLuksVolume().
 */

#include <cmocka_extensions/cmocka_extensions.h>

#include "utest-securememory-create-luks-volume.h"

void cominitSecurememoryCreateLuksVolumeTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    assert_int_not_equal(cominitSecurememoryCreateLuksVolume(NULL), 0);
}
