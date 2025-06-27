// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-parse-pcr-index-failure.c
 * @brief Implementation of several failure case unit tests for cominitTpmParsePcrIndex().
 */
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>
#include <tss2/tss2_tpm2_types.h>

#include "common.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-tpm-parse-pcr-index.h"

void cominitTpmParsePcrIndexTestNonIntegerParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char *argvbuffer = {"test\0-pcr\000bla\0"};
    char **av = calloc(3, sizeof(*av));

    assert_non_null_msg(av, "calloc failed");
    av[0] = argvbuffer;
    av[1] = argvbuffer + 5;
    av[2] = argvbuffer + 10;

    cominitCliArgs_t ctx = {0};

    assert_int_not_equal(cominitTpmParsePcrIndex(&ctx, av[2]), 0);

    assert_false(ctx.pcrSet);

    free(av);
}

void cominitTpmParsePcrIndexTestMixedIntegerParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char *argvbuffer = {"test\0-pcr\0003k\0"};
    char **av = calloc(3, sizeof(*av));

    assert_non_null_msg(av, "calloc failed");
    av[0] = argvbuffer;
    av[1] = argvbuffer + 5;
    av[2] = argvbuffer + 10;

    cominitCliArgs_t ctx = {0};

    assert_int_not_equal(cominitTpmParsePcrIndex(&ctx, av[2]), 0);

    assert_false(ctx.pcrSet);

    free(av);
}

void cominitTpmParsePcrIndexTestNegativeIntegerParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char *argvbuffer = {"test\0-pcr\000-2\0"};
    char **av = calloc(3, sizeof(*av));

    assert_non_null_msg(av, "calloc failed");
    av[0] = argvbuffer;
    av[1] = argvbuffer + 5;
    av[2] = argvbuffer + 10;

    cominitCliArgs_t ctx = {0};

    assert_int_not_equal(cominitTpmParsePcrIndex(&ctx, av[2]), 0);

    assert_false(ctx.pcrSet);

    free(av);
}

void cominitTpmParsePcrIndexTestTooLargeParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    const unsigned int idx = TPM2_PT_PCR_COUNT + 1;
    const size_t pcrCntSize = snprintf(NULL, 0, "%d", idx);
    const size_t lengthOfFixedString = 11;

    char *argvbuffer = calloc(lengthOfFixedString + pcrCntSize, sizeof(*argvbuffer));
    assert_non_null(argvbuffer);

    snprintf(argvbuffer, lengthOfFixedString + pcrCntSize, "test%c-pcr%c%d", '\0', '\0', idx);

    char **av = calloc(3, sizeof(*av));

    assert_non_null_msg(av, "calloc failed");
    av[0] = argvbuffer;
    av[1] = argvbuffer + 5;
    av[2] = argvbuffer + 10;

    cominitCliArgs_t ctx = {0};

    assert_int_not_equal(cominitTpmParsePcrIndex(&ctx, av[2]), 0);

    assert_false(ctx.pcrSet);

    free(av);
    free(argvbuffer);
}

void cominitTpmParsePcrIndexTestTooLargeParamOnEdgeFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    const unsigned int idx = TPM2_PT_PCR_COUNT;
    const size_t pcrCntSize = snprintf(NULL, 0, "%d", idx);
    const size_t lengthOfFixedString = 11;

    char *argvbuffer = calloc(lengthOfFixedString + pcrCntSize, sizeof(*argvbuffer));
    assert_non_null(argvbuffer);

    snprintf(argvbuffer, lengthOfFixedString + pcrCntSize, "test%c-pcr%c%d", '\0', '\0', idx);
    char **av = calloc(3, sizeof(*av));

    assert_non_null_msg(av, "calloc failed");
    av[0] = argvbuffer;
    av[1] = argvbuffer + 5;
    av[2] = argvbuffer + 10;

    cominitCliArgs_t ctx = {0};

    assert_int_not_equal(cominitTpmParsePcrIndex(&ctx, av[2]), 0);

    assert_false(ctx.pcrSet);

    free(av);
    free(argvbuffer);
}
