// SPDX-License-Identifier: MIT
/**
 * @file utest-output-parse-log-level.c
 * @brief Impementation of an cominitOutputParseLogLevel() unit test group using cmocka.
 */
#include "utest-output-parse-log-level.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitOutputParseLogLevel().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitOutputParseLogLevelTestSuccess),
        cmocka_unit_test(cominitOutputParseLogLevelTestFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
