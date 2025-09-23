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
#include "securememory.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-tpm-parse-pcr-index.h"

void cominitTpmParsePcrIndexTestFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    cominitCliArgs_t ctx = {0};
    char negativeInteger[12] = {0};
    char edgeCase[4] = {0};
    char tooLarge[4] = {0};
    snprintf(negativeInteger, sizeof(negativeInteger), "%u", -42);
    snprintf(edgeCase, sizeof(edgeCase), "%d", TPM2_PT_PCR_COUNT);
    snprintf(tooLarge, sizeof(tooLarge), "%d", TPM2_PT_PCR_COUNT + 1);

    const char *testStrings[] = {
        "bla",  // Non integer
        "3k",   // Mixed integer
        negativeInteger,
        edgeCase,
        tooLarge,
    };

    for (size_t i = 0; i < ARRAY_SIZE(testStrings); ++i) {
        assert_int_not_equal(cominitTpmParsePcrIndex(&ctx, testStrings[i]), 0);
    }
}
