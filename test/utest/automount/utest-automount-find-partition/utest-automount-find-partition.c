// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition.c
 * @brief Implementation of an cominitAutomountFindPartition() unit test group using cmocka.
 */
#include "utest-automount-find-partition.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitAutomountFindPartition().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitAutomountFindPartitionTestSuccess),
        cmocka_unit_test(cominitAutomountFindPartitionTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
