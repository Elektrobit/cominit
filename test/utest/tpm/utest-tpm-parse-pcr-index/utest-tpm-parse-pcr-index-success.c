// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-parse-pcr-index-success.c
 * @brief Implementation of a success case unit test for cominitTpmParsePcrIndex().
 */
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "securememory.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-tpm-parse-pcr-index.h"

void cominitTpmParsePcrIndexTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    cominitCliArgs_t ctx = {0};
    char argvalue[4];
    snprintf(argvalue, sizeof(argvalue), "%d", 2);

    assert_int_equal(cominitTpmParsePcrIndex(&ctx, argvalue), 0);

    assert_true(ctx.pcrSet);
    assert_int_equal(ctx.pcrIndex, 2);
}
