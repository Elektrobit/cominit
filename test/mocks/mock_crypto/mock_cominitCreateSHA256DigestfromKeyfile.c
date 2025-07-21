// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCreateSHA256DigestfromKeyfile.c
 * @brief Implementation of a mock function for cominitCreateSHA256DigestfromKeyfile() using cmocka.
 */
#include "mock_cominitCreateSHA256DigestfromKeyfile.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCreateSHA256DigestfromKeyfile(const char *keyfile, unsigned char *digest, size_t digestLen) {
    assert_non_null(keyfile);
    assert_non_null(digest);
    assert_true(digestLen > 0);

    return mock_type(int);
}
