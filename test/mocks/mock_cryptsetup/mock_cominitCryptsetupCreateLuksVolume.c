// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupCreateLuksVolume.c
 * @brief Implementation of a mock function for cominitCryptsetupCreateLuksVolume() using cmocka.
 */
#include "mock_cominitCryptsetupCreateLuksVolume.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupCreateLuksVolume(char *devCrypt, uint8_t *passphrase, size_t passphraseSize) {
    check_expected_ptr(passphrase);
    check_expected_ptr(devCrypt);
    check_expected(passphraseSize);

    return mock_type(int);
}
