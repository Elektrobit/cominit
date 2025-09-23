// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-passphrase.c
 * @brief Implementation of an cominitCryptoCreatePassphrase() unit test group using cmocka.
 */
#include "utest-crypto-create-passphrase.h"

#include "unit_test.h"

/**
 * Run the unit tests for cominitCryptoCreatePassphrase().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitCryptoCreatePassphraseTestSuccess),
        cmocka_unit_test(cominitCryptoCreatePassphraseTestParamFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
