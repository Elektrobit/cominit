// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-verify-signature.c
 * @brief Impementation of an cominitCryptoVerifySignature() unit test group using cmocka.
 */
#include "utest-crypto-verify-signature.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCryptoVerifySignature().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {cmocka_unit_test_setup_teardown(
            cominitCryptoVerifySignatureTestSuccess,
            cominitCryptoVerifySignatureTestSuccessSetup,
            cominitCryptoVerifySignatureTestSuccessTeardown
        ),cmocka_unit_test_setup_teardown(
            cominitCryptoVerifySignatureTestCorruptedDataFailure,
            cominitCryptoVerifySignatureTestCorruptedDataFailureSetup,
            cominitCryptoVerifySignatureTestCorruptedDataFailureTeardown
        ),};
    return cmocka_run_group_tests(tests, NULL, NULL);
}
