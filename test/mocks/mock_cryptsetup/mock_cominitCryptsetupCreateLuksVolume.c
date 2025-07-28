// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupCreateLuksVolume.c
 * @brief Implementation of a mock function for cominitCryptsetupCreateLuksVolume() using cmocka.
 */
#include "mock_cominitCryptsetupCreateLuksVolume.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupCreateLuksVolume(char *devCrypt, TPM2B_DIGEST *passphrase) {
    assert_non_null(devCrypt);
    assert_non_null(passphrase);

    return mock_type(int);
}
