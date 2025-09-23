// SPDX-License-Identifier: MIT
/**
 * @file crypto.h
 * @brief Header related to crypto implementations
 */
#ifndef __CRYPTO_H__
#define __CRYPTO_H__

#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/version.h"

#define SHA256_LEN 32  ///< size of SHA256 digest.

/**
 * Verify data according to a signature and a public key.
 *
 * Uses libmbedcrypto to load the public key from a PEM file, compute the sha256 value of \a data, and verify with
 * \a signature using the RSASSA-PSS algorithm. The signature has to have been generated using sha256+RSASSA-PSS as
 * well.
 *
 * @param data       The data to verify.
 * @param dataLen   The amount of Bytes in \a data.
 * @param signature  sha256/RSASSA-PSS signature of \a data made using \a keyfile.
 * @param keyfile    The path to the public key PEM-file.
 *
 * @return  0 on verification success, -1 otherwise
 */
int cominitCryptoVerifySignature(const uint8_t *data, size_t dataLen, const uint8_t *signature, const char *keyfile);

/**
 * Create a digest by hashing (SHA-256) the public key from a PEM file.
 *
 * @param keyfile   The path to the public key PEM-file.
 * @param digest    Pointer to an allocated buffer capable of holding the bytes given by \a digestLen.
 * @param digestLen The length of the digest.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitCreateSHA256DigestfromKeyfile(const char *keyfile, unsigned char *digest, size_t digestLen);

int cominitCryptoCreatePassphrase(unsigned char *passphrase, size_t passphraseSize);

#endif /* __CRYPTO_H__ */
