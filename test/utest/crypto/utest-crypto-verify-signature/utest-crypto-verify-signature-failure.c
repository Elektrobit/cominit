// SPDX-License-Identifier: MIT
/**
 * @file utest-crypto-verify-signature-failure.c
 * @brief Implementation of a failure case unit test for cominitCryptoVerifySignature().
 */
#include <stdlib.h>

#include "common.h"
#include "crypto.h"
#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/version.h"
#include "unit_test.h"
#include "utest-crypto-verify-signature.h"

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

static const char cominitPrivKey[] =
    "-----BEGIN PRIVATE KEY-----\n"
    "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDDFmLcjFlN1FcK\n"
    "G/Rc4qC80AaOBNF/PFImNTOnMOyltpOtRYDr2Cn82XwBnXf0Of8s1K4/XpQZpnCY\n"
    "d+yiXELP1m0IeiGIIO0gDoea6WbS6XCkXz/lZ+uJYkPQhaIDPiFQfSkUBnJ5YpNb\n"
    "cPC9UbkMRIep8Md8dDXxErfA0I3U2hl+QHRP0vU4PiIPumg8Li0UBdwhJLVybh0Q\n"
    "+ioYwOrTldLjXcUjr9p4K3EtCAQiYCCcXKtc4ysvZAwN+ycD9TeN68gX0g1hFEZ6\n"
    "4I0swZJPPkEz2nf0Vl22jiy4MyNg9u4lHVSFMG9AKtqk2Sf53YsjDl1KGmDx43gB\n"
    "by8zx1UtAgMBAAECggEAT+ANkEGCmwQysvfpIFLF/yipVmKQqCi5dd1Yjb7RVbQg\n"
    "WNRZSbcS0ez2Ae9IWt2YJfHaeiWuO6ZS+Yo4f+2IiXJA6DShIAxjKjnoubuQlxBv\n"
    "R02casAWxwRNwcXFJNC+cScdt45ANoTu0oS2fL1GJ8izRxhHhSRLpa3FQJPjTl9Z\n"
    "tEEIfldmbF7vjbq4zmryTcfibYKn9BtITg46zRPWV7sX8qyuGjvnyYQvI2OD6BJr\n"
    "lSsePP04FarrCZnNsXUmPmbvopF//GUlmeRjrs78/OxNmTNaxE5H7m6nILy6c2cB\n"
    "EfX3l3w4QFb3lb6CsAWULTPVurvMivPrkF/gCo0A3QKBgQD8K5jmaIv4R+YWyHQ8\n"
    "H0QozobIdrFn+5GoGw31ncL6i+RWhVdmyAcvUg5gVnzjVe34ujxPxWp8blhu607d\n"
    "W4K8mgAiCn1mSBkW+w8MIfQZMSM0t/3h9pvFqnfYeD5PcIHEu8yTmNUDHa/iGU4q\n"
    "9Dkgh4FTR/u6JfeTTuJHUdk3AwKBgQDGDNvZIGqTiaWKzMCjdCBJILdMBIpbh3zs\n"
    "jCbVKwIHB1kbE1hwIf79n1XVwgQA2fih3zgqvkv+iGJlnUeErEiyHsZQ+/c3p/aY\n"
    "uwRoGfmjU1kuVpZgs4DfJ71SDUJykVqMf8/CyfS2RLz5A+yGtwHJjoJMEh01UweL\n"
    "2oVYg3W0DwKBgQDTMhBUqKrRviXOXwQ6dlU3eAEaU44xcFiFWmNOmlp6TD9o1zRk\n"
    "OJML5sVzmfvuykEMV627gbO1kxUtkJX0fnf4TWJfIDn2N9wLiykHGEi/8I9zz8Uq\n"
    "QsgjvKmy+sH09l04z7B+zTlZ6rsrudXl07UksVh2z4rcVIANN7v7z2kE0QKBgAQL\n"
    "GynxqXtdog2VZJFxu38CtftNVG3N9FT3/4r2l9IA6mbNHE8dwpD3Ac+daGuEB7l2\n"
    "TzVAMVj++9NLRLyGr+1DB5V7r25pdXVkzBA8B4a4GugBSIys3YKw0wKr1qF0Kj8Y\n"
    "FOvLJ73Y5WrIE7okXlr5F9vpusLt8Qd/7J0EAhEtAoGBAKHG/emQ+WpiZqh5TeiX\n"
    "pAnjuTqTw8eS05kxhUvGTAHFJ40kllSa5fP1gSaoKb+CxQFGljRsREVvfSW69rk2\n"
    "SR+nI0yWULJsUNDAm52TdhacZGg35ghI08DIpqupJPmLEXlbWezV57iKBlJL3biX\n"
    "oK/xnXKWwB9ovP27Nud4R0mN\n"
    "-----END PRIVATE KEY-----\n";

/* Generate signature:
 *
 * printf "1234" | \
 *  openssl dgst -sha256 \
 *    -sigopt rsa_padding_mode:pss \
 *    -sigopt rsa_pss_saltlen:-1 \
 *    -sigopt rsa_mgf1_md:sha256 \
 *    -sign private.pem \
 *  > signature
 *
 *  xxd -i signature > signature.hex
 */
static unsigned char cominitSignature[] = {
    0xb4, 0xb9, 0x78, 0x3e, 0x77, 0x2e, 0x82, 0xd5, 0xce, 0x05, 0x10, 0x0c, 0xf8, 0x41, 0xea, 0x70, 0xdb, 0xb0, 0x24,
    0x41, 0x9f, 0xcd, 0x6e, 0x1f, 0x92, 0xd7, 0x7c, 0xf1, 0xba, 0xec, 0x5a, 0x73, 0xb9, 0x26, 0x69, 0xb6, 0x06, 0xcd,
    0x6e, 0xb0, 0x8e, 0x7f, 0xe6, 0x77, 0xc3, 0xf8, 0xd8, 0x89, 0xe7, 0xb0, 0x1d, 0x52, 0xf2, 0xf9, 0x92, 0x41, 0x2e,
    0x10, 0xbf, 0x28, 0x68, 0xac, 0x27, 0x7e, 0x2f, 0x29, 0x8d, 0xc1, 0x31, 0x7c, 0x77, 0x70, 0xf5, 0x7d, 0x27, 0xea,
    0x55, 0x81, 0xab, 0x90, 0x73, 0x97, 0xe2, 0x03, 0xb8, 0x4b, 0x44, 0x0a, 0x3c, 0x5c, 0xa5, 0x3f, 0xd2, 0x0d, 0x34,
    0xaf, 0x2f, 0x17, 0x9e, 0x51, 0x7b, 0x98, 0x26, 0xe7, 0x5a, 0x5d, 0xc1, 0xb6, 0x72, 0xbd, 0x37, 0xe8, 0x5c, 0xec,
    0x41, 0xfe, 0x20, 0x87, 0x69, 0xec, 0xd7, 0x2c, 0x06, 0x06, 0x6e, 0xe3, 0x18, 0x31, 0xe5, 0xb6, 0xb5, 0xf9, 0x32,
    0x83, 0x84, 0x21, 0x97, 0x25, 0xcf, 0x3b, 0xc2, 0xa3, 0x2c, 0xbd, 0xdf, 0x6f, 0xf5, 0xc6, 0xa8, 0x29, 0x5f, 0xf3,
    0x0c, 0x90, 0xe3, 0xf1, 0xdd, 0xc3, 0x33, 0x1a, 0xf2, 0x5d, 0x77, 0xc8, 0xdc, 0x55, 0x97, 0xf1, 0xa8, 0xbb, 0x9c,
    0xf5, 0xee, 0xbb, 0x77, 0x4a, 0x9c, 0x46, 0x54, 0x4b, 0x5f, 0x32, 0x30, 0x80, 0x2c, 0x9b, 0xd7, 0xad, 0x1b, 0x47,
    0x64, 0x39, 0xb8, 0x1d, 0x4b, 0x03, 0xca, 0xd3, 0x3d, 0x24, 0x1a, 0x61, 0x15, 0x2e, 0xb0, 0x36, 0xd0, 0xfe, 0x7d,
    0xe1, 0x10, 0x96, 0x28, 0xa3, 0x7f, 0x3c, 0x61, 0xa2, 0xbe, 0x1b, 0x3c, 0xfe, 0xd2, 0xcd, 0x8f, 0x4a, 0x34, 0xe0,
    0xc8, 0x0e, 0xf8, 0xf3, 0x56, 0x18, 0xa3, 0x48, 0x4f, 0xd8, 0x46, 0x03, 0x12, 0x89, 0xf8, 0xdf, 0xf8, 0x71, 0xc3,
    0x85, 0x2d, 0xbf, 0x92, 0xa6, 0xc7, 0xe0, 0x9c, 0xae};

int cominitCryptoVerifySignatureTestCorruptedDataFailureSetup(void **state) {
    struct testContext *testCtx = NULL;

    testCtx = calloc(1, sizeof(struct testContext));
    if (testCtx != NULL) {
        char template[] = "/tmp/keyfile-XXXXXX";
        testCtx->fd = mkstemp(template);
        testCtx->keyfile = strdup(template);
        if (write(testCtx->fd, cominitPubKey, strlen(cominitPubKey)) != (ssize_t)strlen(cominitPubKey)) {
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

int cominitCryptoVerifySignatureTestCorruptedDataFailureTeardown(void **state) {
    struct testContext *testCtx = *state;

    if (testCtx != NULL) {
        if (testCtx->keyfile != NULL) {
            unlink(testCtx->keyfile);
            free(testCtx->keyfile);
        }
        free(testCtx);
    }

    return 0;
}

void cominitCryptoVerifySignatureTestCorruptedDataFailure(void **state) {
    COMINIT_PARAM_UNUSED(cominitPrivKey);
    struct testContext *testCtx = *state;
    unsigned char corruptedData[] = "12345";
    size_t len = strlen((char *)corruptedData);

    assert_int_not_equal(cominitCryptoVerifySignature(corruptedData, len, cominitSignature, testCtx->keyfile), 0);
}
