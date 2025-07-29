// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-digest-failure.c
 * @brief Implementation of a failure case unit test for cominitCreateSHA256DigestfromKeyfile().
 */
#include <stdlib.h>

#include "common.h"
#include "crypto.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/version.h"
#include "unit_test.h"
#include "utest-crypto-create-digest.h"

void cominitCryptoCreateDigestTestDigestNullFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    unsigned char *digest = NULL;
    const char keyfile[] = {0};

    assert_int_not_equal(cominitCreateSHA256DigestfromKeyfile(keyfile, digest, SHA256_LEN), 0);
}

void cominitCryptoCreateDigestTestKeyfileNullFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    unsigned char digest[SHA256_LEN] = {0};
    const char *keyfile = NULL;

    assert_int_not_equal(cominitCreateSHA256DigestfromKeyfile(keyfile, digest, SHA256_LEN), 0);
}

void cominitCryptoCreateDigestTestDigestTooSmallFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    unsigned char digest[1] = {0};
    const char keyfile[] = {0};

    assert_int_not_equal(cominitCreateSHA256DigestfromKeyfile(keyfile, digest, sizeof(digest)), 0);
}
