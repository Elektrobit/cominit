// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-unseal.c
 * @brief Implementation of an cominitSecurememoryEsysUnseal() unit test group using cmocka.
 */
#include "utest-securememory-esys-unseal.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitSecurememoryEsysUnseal().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitSecurememoryEsysUnsealTestSuccess),
        cmocka_unit_test(cominitSecurememoryEsysUnsealTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
