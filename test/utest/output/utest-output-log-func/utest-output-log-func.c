// SPDX-License-Identifier: MIT
/**
 * @file utest-output-log-func.c
 * @brief Impementation of an cominitOutputLogFunc() unit test group using cmocka.
 */
#include "utest-output-log-func.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitOutputLogFunc().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitOutputLogFuncTestSuccessPrint),
        cmocka_unit_test(cominitOutputLogFuncTestSuccessNoPrint),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
