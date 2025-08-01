// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-add-token-success.c
 * @brief Implementation of a success case unit test for cominitCryptsetupAddToken().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "utest-cryptsetup-add-token.h"

void cominitCryptsetupAddTokenTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char devCryptTest[] = "/dev/crypt";

    expect_string(__wrap_cominitSubprocessSpawn, path, COMINIT_CRYPTSETUP_DIR);
    expect_any(__wrap_cominitSubprocessSpawn, argv);
    expect_any(__wrap_cominitSubprocessSpawn, env);

    will_return(__wrap_cominitSubprocessSpawn, 0);

    assert_int_equal(cominitCryptsetupAddToken(devCryptTest), 0);
}
