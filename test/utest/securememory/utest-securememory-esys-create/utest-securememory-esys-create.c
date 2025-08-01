// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-create.c
 * @brief Implementation of an cominitSecurememoryEsysCreate() unit test group using cmocka.
 */
#include "utest-securememory-esys-create.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitSecurememoryEsysCreate().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitSecurememoryEsysCreateTestSuccess),
        cmocka_unit_test(cominitSecurememoryEsysCreateTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
