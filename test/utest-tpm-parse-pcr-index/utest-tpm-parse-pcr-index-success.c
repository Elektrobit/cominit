// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-parse-pcr-index-success.c
 * @brief Implementation of a success case unit test for cominitTpmParsePcrIndex().
 */
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdlib.h>

#include "common.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-tpm-parse-pcr-index.h"

void cominitTpmParsePcrIndexTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char *argvbuffer = {"test\0-pcr\0002\0"};
    char **av = calloc(3, sizeof(*av));

    assert_non_null_msg(av, "calloc failed");
    av[0] = argvbuffer;
    av[1] = argvbuffer + 5;
    av[2] = argvbuffer + 10;

    cominitCliArgs_t ctx = {0};

    assert_int_equal(cominitTpmParsePcrIndex(&ctx, av[2]), 0);

    assert_true(ctx.pcrSet);
    assert_int_equal(ctx.pcrIndex, 2);

    free(av);
}
