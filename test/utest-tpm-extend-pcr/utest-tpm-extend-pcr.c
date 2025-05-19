// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-extend-pcr.c
 * @brief Impementation of an cominitTpmParsePcrIndex() unit test group using cmocka.
 */
#include "utest-tpm-extend-pcr.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitTpmExtendPCR().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitTpmExtendPCRTestIdx0Success),
        cmocka_unit_test(cominitTpmExtendPCRTestIdx2Success),
        cmocka_unit_test(cominitTpmExtendPCRTestNullCtxFailure),
        cmocka_unit_test(cominitTpmExtendPCRTestNullKeyfileFailure),
        cmocka_unit_test(cominitTpmExtendPCRTestNegativeIntegerParamFailure),
        cmocka_unit_test(cominitTpmExtendPCRTestTooLargeParamFailure),
        cmocka_unit_test(cominitTpmExtendPCRTestTooLargeParamOnEdgeFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
