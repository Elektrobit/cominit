// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-open-luks-volume.c
 * @brief Implementation of an cominitCryptsetupOpenLuksVolume() unit test group using cmocka.
 */
#include "utest-cryptsetup-open-luks-volume.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCryptsetupOpenLuksVolume().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitCryptsetupOpenLuksVolumeTestSuccess),
        cmocka_unit_test(cominitCryptsetupOpenLuksVolumeTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
