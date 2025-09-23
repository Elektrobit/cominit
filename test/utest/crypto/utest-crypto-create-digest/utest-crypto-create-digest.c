// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-digest.c
 * @brief Impementation of an cominitCryptoCreateDigest() unit test group using cmocka.
 */
#include "utest-crypto-create-digest.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCryptoCreateDigest().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test_setup_teardown(cominitCryptoCreateDigestTestSuccess, cominitCryptoCreateDigestTestSuccessSetup,
                                        cominitCryptoCreateDigestTestSuccessTeardown),
        cmocka_unit_test(cominitCryptoCreateDigestTestDigestNullFailure),
        cmocka_unit_test(cominitCryptoCreateDigestTestKeyfileNullFailure),
        cmocka_unit_test(cominitCryptoCreateDigestTestDigestTooSmallFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
