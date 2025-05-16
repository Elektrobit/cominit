#include "tpm.h"

#include <errno.h>
#include <mbedtls/pk.h>
#include <mbedtls/sha256.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "meta.h"
#include "output.h"

#define DER_BUFFER_SIZE 1600  ///< buffer size to hold RSA‑4k key.
#define SHA256_LEN 32         ///< size of SHA256 digest.

/**
 * Checks whether the TPM driver module is loaded and loads it if needed.
 *
 * Dummy Implementation: needs to be implemented in next tasks.
 *
 * @return  always 1
 */
static int cominitTpmLoadDriver() {
    return EXIT_SUCCESS;
}

/**
 * Checks whether the TPM driver module is functional.
 *
 * @param tpmCtx   The TPM context.
 *
 * @return  0 on success, -1 otherwise
 */
static int cominitTpmSelftest(cominitTpmContext_t *tpmCtx) {
    int result = EXIT_FAILURE;
    TSS2_RC rc = Esys_SelfTest(tpmCtx->esysCtx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, TPM2_YES);
    if (rc != TSS2_RC_SUCCESS) {
        cominitErrPrint("Selftest failed");
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

int cominitInitTpm(cominitTpmContext_t *tpmCtx) {
    const char *tctiConf = "device:/dev/tpm0";
    int result = EXIT_FAILURE;

    if (tpmCtx == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        result = cominitTpmLoadDriver();
        if (result == EXIT_SUCCESS) {
            result = EXIT_FAILURE;
            TSS2_RC rc = Tss2_TctiLdr_Initialize(tctiConf, &tpmCtx->tctiCtx);
            if (rc != TSS2_RC_SUCCESS) {
                cominitErrPrint("Initializing TCTI context failed");
            } else {
                rc = Esys_Initialize(&tpmCtx->esysCtx, tpmCtx->tctiCtx, NULL);
                if (rc != TSS2_RC_SUCCESS) {
                    cominitErrPrint("Initializing ESYS context failed");
                } else {
                    result = cominitTpmSelftest(tpmCtx);
                }
            }
        }
    }

    return result;
}

int cominitTpmExtendPCR(cominitTpmContext_t *tpmCtx, const char *keyfile, unsigned long pcrIndex) {
    int result = EXIT_FAILURE;
    int err = -1;
    unsigned char digest[SHA256_LEN];
    mbedtls_pk_context pkCtx;
    TSS2_RC rc = TSS2_ESYS_RC_GENERAL_FAILURE;

    if (tpmCtx == NULL || keyfile == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        mbedtls_pk_init(&pkCtx);
        err = mbedtls_pk_parse_public_keyfile(&pkCtx, keyfile);

        if (err == 0) {
            unsigned char der[DER_BUFFER_SIZE];
            int derLen = mbedtls_pk_write_pubkey_der(&pkCtx, der, sizeof(der));
            if (derLen > 0) {
                const unsigned char *pubKeyDer = der + sizeof(der) - derLen;
                err = mbedtls_sha256_ret(pubKeyDer, (size_t)derLen, digest, 0);
                if (err == 0) {
                    ESYS_TR pcrTR = ESYS_TR_PCR0 + pcrIndex;
                    TPML_DIGEST_VALUES vals = {.count = 1};
                    vals.digests[0].hashAlg = TPM2_ALG_SHA256;
                    memcpy(vals.digests[0].digest.sha256, digest, sizeof(digest));

                    rc = Esys_PCR_Extend(tpmCtx->esysCtx, pcrTR, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE, &vals);
                }
            } else {
                err = -1;
            }
        }

        mbedtls_pk_free(&pkCtx);
    }

    if (err != 0) {
        cominitErrPrint("Could not hash the rootfs public key");
    }

    if (rc == TSS2_RC_SUCCESS) {
        result = EXIT_SUCCESS;
    }

    return result;
}

int cominitDeleteTpm(cominitTpmContext_t *tpmCtx) {
    int result = EXIT_FAILURE;

    if (tpmCtx == NULL || tpmCtx->tctiCtx == NULL || tpmCtx->esysCtx == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        Tss2_TctiLdr_Finalize(&tpmCtx->tctiCtx);
        Esys_Finalize(&tpmCtx->esysCtx);
        result = EXIT_SUCCESS;
    }

    return result;
}

int cominitTpmParsePcrIndex(cominitCliArgs_t *ctx, const char *argValue) {
    int result = EXIT_FAILURE;

    if (ctx == NULL || argValue == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        ctx->pcrSet = false;
        errno = 0;
        char *end;
        unsigned long pcrIndex = strtoul(argValue, &end, 0);
        if (!errno && *end == '\0' && pcrIndex < TPM2_PT_PCR_COUNT) {
            result = EXIT_SUCCESS;
            ctx->pcrSet = true;
            ctx->pcrIndex = pcrIndex;
        }
    }

    return result;
}
