// SPDX-License-Identifier: MIT
/**
 * @file utest-output-parse-log-level-success.c
 * @brief Implementation of a success case unit test for cominitOutputParseLogLevel().
 */
#include <cmocka_extensions/cmocka_extensions.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "output.h"
#include "unit_test.h"
#include "utest-output-parse-log-level.h"

void cominitOutputParseLogLevelTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    cominitCliArgs_t ctx = {0};
    char argvalue[4];
    snprintf(argvalue, sizeof(argvalue), "%d", COMINIT_LOG_LEVEL_WARN);

    assert_int_equal(cominitOutputParseLogLevel(&ctx.visibleLogLevel, argvalue), 0);

    assert_int_equal(ctx.visibleLogLevel, COMINIT_LOG_LEVEL_WARN);
}
