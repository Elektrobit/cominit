// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupAddToken.c
 * @brief Implementation of a mock function for cominitCryptsetupAddToken() using cmocka.
 */
#include "mock_cominitCryptsetupAddToken.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupAddToken(char *devCrypt) {
    check_expected_ptr(devCrypt);

    return mock_type(int);
}
