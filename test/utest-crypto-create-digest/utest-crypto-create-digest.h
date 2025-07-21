// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-digest.h
 * @brief Header declaring cmocka unit test functions for cominitCreateSHA256DigestfromKeyfile().
 */
#ifndef __UTEST_CRYPTO_CREATE_DIGEST_H__
#define __UTEST_CRYPTO_CREATE_DIGEST_H__

#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

struct testContext {
    int fd;
    char *keyfile;
};

/**
 * Unit test for cominitCreateSHA256DigestfromKeyfile() successful code path.
 * @param state
 */
void cominitCryptoCreateDigestTestSuccess(void **state);
int cominitCryptoCreateDigestTestSuccessSetup(void **state);
int cominitCryptoCreateDigestTestSuccessTeardown(void **state);

/**
 * Unit test for cominitCreateSHA256DigestfromKeyfile() with Digest too small to hold sha256 hash.
 * @param state
 */
void cominitCryptoCreateDigestTestDigestTooSmallFailure(void **state);

/**
 * Unit test for cominitCreateSHA256DigestfromKeyfile() with Digest pointing to NULL.
 * @param state
 */
void cominitCryptoCreateDigestTestDigestNullFailure(void **state);

/**
 * Unit test for cominitCreateSHA256DigestfromKeyfile() with Keyfile pointing to NULL.
 * @param state
 */
void cominitCryptoCreateDigestTestKeyfileNullFailure(void **state);

#endif /* __UTEST_CRYPTO_CREATE_DIGEST_H__ */
