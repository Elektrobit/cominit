// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptoCreatePassphrase.c
 * @brief Implementation of a mock function for cominitCryptoCreatePassphrase() using cmocka.
 */
#include "mock_cominitCryptoCreatePassphrase.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptoCreatePassphrase(unsigned char *passphrase, size_t passphraseSize) {
    check_expected_ptr(passphrase);
    check_expected(passphraseSize);

    return mock_type(int);
}
