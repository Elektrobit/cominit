// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-verify-signature.h
 * @brief Header declaring cmocka unit test functions for cominitCreateSHA256DigestfromKeyfile().
 */
#ifndef __UTEST_CRYPTO_CREATE_DIGEST_H__
#define __UTEST_CRYPTO_CREATE_DIGEST_H__

#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

struct testContext {
    int  fd;
    char *keyfile;
};

/**
 * Unit test for cominitCreateSHA256DigestfromKeyfile() successful code path.
 * @param state
 */
void cominitCryptoVerifySignatureTestSuccess(void **state);
int cominitCryptoVerifySignatureTestSuccessSetup(void **state);
int cominitCryptoVerifySignatureTestSuccessTeardown(void **state);

/**
 * Unit test for cominitCreateSHA256DigestfromKeyfile() with corrupted data.
 * @param state
 */
void cominitCryptoVerifySignatureTestCorruptedDataFailure(void **state);
int cominitCryptoVerifySignatureTestCorruptedDataFailureSetup(void **state);
int cominitCryptoVerifySignatureTestCorruptedDataFailureTeardown(void **state);


#endif /* __UTEST_CRYPTO_CREATE_DIGEST_H__ */
