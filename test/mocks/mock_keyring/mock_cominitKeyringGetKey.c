// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitKeyringGetKey.c
 * @brief Implementation of a mock function for cominitKeyringGetKey() using cmocka.
 */
#include "mock_cominitKeyringGetKey.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitKeyringGetKey(uint8_t *key, size_t keyMaxLen, char *keyDesc) {
    check_expected_ptr(key);
    check_expected_ptr(keyDesc);
    check_expected(keyMaxLen);

    return mock_type(int);
}
