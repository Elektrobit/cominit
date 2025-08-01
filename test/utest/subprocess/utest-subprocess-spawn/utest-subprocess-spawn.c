// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn.c
 * @brief Implementation of an cominitSubprocessSpawn() unit test group using cmocka.
 */
#include "utest-subprocess-spawn.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitSubprocessSpawn().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitSubprocessSpawnTestSuccess),
        cmocka_unit_test(cominitSubprocessSpawnTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
