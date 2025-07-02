// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-create-digest-success.c
 * @brief Implementation of a success case unit test for cominitCreateSHA256DigestfromKeyfile().
 */
#include <stdlib.h>

#include "common.h"
#include "crypto.h"
#include "unit_test.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/version.h"
#include "utest-crypto-create-digest.h"

static const char cominitPubKey[] =
    "-----BEGIN PUBLIC KEY-----\n"
    "MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEAwxZi3IxZTdRXChv0XOKg\n"
    "vNAGjgTRfzxSJjUzpzDspbaTrUWA69gp/Nl8AZ139Dn/LNSuP16UGaZwmHfsolxC\n"
    "z9ZtCHohiCDtIA6Hmulm0ulwpF8/5WfriWJD0IWiAz4hUH0pFAZyeWKTW3DwvVG5\n"
    "DESHqfDHfHQ18RK3wNCN1NoZfkB0T9L1OD4iD7poPC4tFAXcISS1cm4dEPoqGMDq\n"
    "05XS413FI6/aeCtxLQgEImAgnFyrXOMrL2QMDfsnA/U3jevIF9INYRRGeuCNLMGS\n"
    "Tz5BM9p39FZdto4suDMjYPbuJR1UhTBvQCrapNkn+d2LIw5dShpg8eN4AW8vM8dV\n"
    "LQIDAQAB\n"
    "-----END PUBLIC KEY-----\n";

int cominitCryptoCreateDigestTestSuccessSetup(void **state) {
    struct testContext *testCtx = NULL;

    testCtx = calloc(1, sizeof(struct testContext));
    if(testCtx != NULL) {
        char template[] = "/tmp/keyfile-XXXXXX";
        testCtx->fd = mkstemp(template);
        testCtx->keyfile = strdup(template);
        if (write(testCtx->fd, cominitPubKey, strlen(cominitPubKey)) != (ssize_t)strlen(cominitPubKey)){
            close(testCtx->fd);
            free(testCtx->keyfile);
            free(testCtx);
            return errno;
        };
        *state = testCtx;
        close(testCtx->fd);
        return 0;
    }

    return -1;
}

int cominitCryptoCreateDigestTestSuccessTeardown(void **state) {
    struct testContext *testCtx = *state;

    if (testCtx != NULL) {
        if(testCtx->keyfile != NULL){
            unlink(testCtx->keyfile);
            free(testCtx->keyfile);
        }
        free(testCtx);
    }

    return 0;
}

void cominitCryptoCreateDigestTestSuccess(void **state) {
    struct testContext *testCtx = *state;
    unsigned char digest[SHA256_LEN] = {0};

    assert_int_equal(cominitCreateSHA256DigestfromKeyfile(testCtx->keyfile, digest, SHA256_LEN), 0);
}
