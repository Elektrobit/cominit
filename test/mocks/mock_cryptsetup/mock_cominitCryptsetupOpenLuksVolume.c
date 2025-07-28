// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupCreateLuksVolume.c
 * @brief Implementation of a mock function for cominitCryptsetupCreateLuksVolume() using cmocka.
 */
#include "mock_cominitCryptsetupCreateLuksVolume.h"
#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupOpenLuksVolume(char *devCrypt, TPM2B_DIGEST *key) {
    assert_non_null(devCrypt);
    assert_non_null(key);

    return mock_type(int);
}
