// SPDX-License-Identifier: MIT
/**
 * @file utest-delete-tpm.c
 * @brief Impementation of an cominitDeleteTpm() unit test group using cmocka.
 */
#include "utest-delete-tpm.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitInitTpm().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitDeleteTpmTestSuccess),
        cmocka_unit_test(cominitDeleteTpmTestNullCtxFailure),
        cmocka_unit_test(cominitDeleteTpmTestTss2NullFailure),
        cmocka_unit_test(cominitDeleteTpmTestEsysNullFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
