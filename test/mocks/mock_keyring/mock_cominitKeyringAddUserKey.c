// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitKeyringAddUserKey.c
 * @brief Implementation of a mock function for cominitKeyringGetKey() using cmocka.
 */
#include "mock_cominitKeyringAddUserKey.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitKeyringAddUserKey(const char *keyDesc, const uint8_t *key, size_t keyLen) {
    check_expected_ptr(keyDesc);
    check_expected_ptr(key);
    check_expected(keyLen);

    return mock_type(int);
}
