// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-create-luks-volume.c
 * @brief Implementation of an cominitSecurememoryCreateLuksVolume() unit test group using cmocka.
 */
#include "utest-securememory-create-luks-volume.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitSecurememoryCreateLuksVolume().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitSecurememoryCreateLuksVolumeTestSuccess),
        cmocka_unit_test(cominitSecurememoryCreateLuksVolumeTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
