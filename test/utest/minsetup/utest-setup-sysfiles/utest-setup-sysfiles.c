// SPDX-License-Identifier: MIT
/**
 * @file utest-setup-sysfiles.c
 * @brief Impementation of an cominitSetupSysfiles() unit test group using cmocka.
 */
#include "utest-setup-sysfiles.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitSetupSysfiles().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitSetupSysfilesTestSuccess), cmocka_unit_test(cominitSetupSysfilesTestSuccessDirExists),
        cmocka_unit_test(cominitSetupSysfilesTestSuccessAlreadyMounted),
        cmocka_unit_test(cominitSetupSysfilesTestMkdirError), cmocka_unit_test(cominitSetupSysfilesTestMountError)};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
