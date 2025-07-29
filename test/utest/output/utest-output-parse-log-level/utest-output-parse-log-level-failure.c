// SPDX-License-Identifier: MIT
/**
 * @file utest-output-parse-log-level-failure.c
 * @brief Implementation of several failure case unit tests for cominitOutputParseLogLevel().
 */
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "output.h"
#include "unit_test.h"
#include "utest-output-parse-log-level.h"

void cominitOutputParseLogLevelTestFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    cominitLogLevelE_t visibleLogLevel = 0;
    char negativeInteger[12] = {0};
    char edgeCase[4] = {0};
    char tooLarge[4] = {0};
    snprintf(negativeInteger, sizeof(negativeInteger), "%u", -42);
    snprintf(edgeCase, sizeof(edgeCase), "%d", COMINIT_LOG_LEVEL_COUNT);
    snprintf(tooLarge, sizeof(tooLarge), "%d", COMINIT_LOG_LEVEL_INVALID);

    const char *testStrings[] = {
        "bla",  // Non integer
        "3k",   // Mixed integer
        negativeInteger,
        edgeCase,
        tooLarge,
    };

    for (size_t i = 0; i < ARRAY_SIZE(testStrings); ++i) {
        assert_int_not_equal(cominitOutputParseLogLevel(&visibleLogLevel, testStrings[i]), 0);
    }
}
