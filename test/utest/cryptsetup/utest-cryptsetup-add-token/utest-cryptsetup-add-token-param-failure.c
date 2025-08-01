// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-add-token-param-failure.c
 * @brief Implementation of a failure case unit test for cominitCryptsetupAddToken().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tss2/tss2_esys.h>
#include <unistd.h>

#include "tpm.h"
#include "utest-cryptsetup-add-token.h"

void cominitCryptsetupAddTokenTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    assert_int_not_equal(cominitCryptsetupAddToken(NULL), 0);
}
