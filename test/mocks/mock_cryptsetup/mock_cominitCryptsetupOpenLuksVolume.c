// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupOpenLuksVolume.c
 * @brief Implementation of a mock function for cominitCryptsetupOpenLuksVolume() using cmocka.
 */
#include "mock_cominitCryptsetupOpenLuksVolume.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupOpenLuksVolume(char *devCrypt) {
    check_expected_ptr(devCrypt);

    return mock_type(int);
}
