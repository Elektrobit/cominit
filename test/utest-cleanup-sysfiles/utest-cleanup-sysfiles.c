// SPDX-License-Identifier: MIT
/**
 * @file utest-cleanup-sysfiles.c
 * @brief Impementation of an cominitCleanupSysfiles() unit test group using cmocka.
 */
#include "utest-cleanup-sysfiles.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCleanupSysfiles().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {cmocka_unit_test(cominitCleanupSysfilesTestSuccess),
                                       cmocka_unit_test(cominitCleanupSysfilesTestUmountError)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
