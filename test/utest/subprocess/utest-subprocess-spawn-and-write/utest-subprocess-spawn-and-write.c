// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn-and-write.c
 * @brief Implementation of an cominitSubprocessSpawnAndWrite() unit test group using cmocka.
 */
#include "utest-subprocess-spawn-and-write.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitSubprocessSpawnAndWrite().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitSubprocessSpawnAndWriteTestSuccess),
        cmocka_unit_test(cominitSubprocessSpawnAndWriteTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
