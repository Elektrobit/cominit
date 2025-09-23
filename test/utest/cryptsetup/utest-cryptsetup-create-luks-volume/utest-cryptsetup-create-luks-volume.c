// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-create-luks-volume.c
 * @brief Implementation of an cominitCryptsetupCreateLuksVolume() unit test group using cmocka.
 */
#include "utest-cryptsetup-create-luks-volume.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCryptsetupCreateLuksVolume().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitCryptsetupCreateLuksVolumeTestSuccess),
        cmocka_unit_test(cominitCryptsetupCreateLuksVolumeTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
