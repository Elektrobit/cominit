// SPDX-License-Identifier: MIT
/**
 * @file utest-init-tpm.c
 * @brief Impementation of an cominitTpmParsePcrIndex() unit test group using cmocka.
 */
#include "utest-init-tpm.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitInitTpm().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitInitTpmTestSuccess),
        cmocka_unit_test(cominitInitTpmTestNullCtxFailure),
        cmocka_unit_test(cominitInitTpmTestTss2InitFailFailure),
        cmocka_unit_test(cominitInitTpmTestEsysInitFailFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
