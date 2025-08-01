// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-add-token.c
 * @brief Implementation of an cominitCryptsetupAddToken() unit test group using cmocka.
 */
#include "utest-cryptsetup-add-token.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCryptsetupAddToken().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitCryptsetupAddTokenTestSuccess),
        cmocka_unit_test(cominitCryptsetupAddTokenTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
