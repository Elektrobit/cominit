// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-parse-pcr-index.c
 * @brief Impementation of an cominitTpmParsePcrIndex() unit test group using cmocka.
 */
#include "utest-tpm-parse-pcr-index.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitTpmParsePcrIndex().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitTpmParsePcrIndexTestSuccess),
        cmocka_unit_test(cominitTpmParsePcrIndexTestFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
