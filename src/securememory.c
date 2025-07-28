#include "securememory.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

#include "crypto.h"
#include "cryptsetup.h"
#include "keyring.h"

/**
 * Overwrite a memory region with zeros
 *
 * @param bufferToClear Pointer to the memory to clear
 * @param bufferSize     Number of bytes to check.
 *
 * @return 1 if every byte is 0x00, 0 otherwise.
 */
static int cominitSecurememoryZeroOut(void *bufferToClear, size_t bufferSize) {
    int result = EXIT_FAILURE;

    if (bufferToClear == NULL || bufferSize <= 0) {
        cominitErrPrint("Invalid parameters");
    } else {
        volatile uint8_t *buffer = bufferToClear;
        for (size_t i = 0; i < bufferSize; i++) {
            buffer[i] = 0;
        }
        result = EXIT_SUCCESS;
    }

    return result;
}

/**
 * Verify that a buffer is completely zeroed.
 *
 * @param bufferToVerify Pointer to the memory to check
 * @param bufferSize     Number of bytes to check.
 *
 * @return 1 if every byte is 0x00, 0 otherwise.
 */
static int cominitSecurememoryVerifyZero(const void *bufferToVerify, size_t bufferSize) {
    int result = EXIT_SUCCESS;

    if (bufferToVerify == NULL || bufferSize == 0) {
        result = EXIT_FAILURE;
        cominitErrPrint("Invalid parameters");
    } else {
        const uint8_t *buffer = bufferToVerify;
        for (size_t i = 0; i < bufferSize; i++) {
            if (buffer[i] != 0) {
                result = EXIT_FAILURE;
                break;
            }
        }
    }

    return result;
}

cominitTpmState_t cominitSecurememoryEsysUnseal(ESYS_CONTEXT *ectx, ESYS_TR *blobHandle, ESYS_TR *session) {
    TPM2B_SENSITIVE_DATA *keyBuffer = NULL;
    cominitTpmState_t state = TpmFailure;

    if (ectx == NULL || blobHandle == NULL || session == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        TSS2_RC rc = Esys_Unseal(ectx, *blobHandle, *session, ESYS_TR_NONE, ESYS_TR_NONE, &keyBuffer);
        if (rc != TSS2_RC_SUCCESS) {
            if (rc == POLICY_FAILURE_RC) {
                state = TpmPolicyFailure;
            } else {
                cominitErrPrint("Unsealing failed");
            }
        } else {
            if (mlock(keyBuffer, sizeof(TPM2B_SENSITIVE_DATA)) != 0) {
                cominitErrnoPrint("mlock failed");
            } else {
                if (cominitKeyringAddUserKey(COMINIT_TPM_SECURE_STORAGE_KEY_NAME, keyBuffer->buffer, keyBuffer->size) ==
                    0) {
                    state = Unsealed;
                }
                if (cominitSecurememoryZeroOut(keyBuffer->buffer, keyBuffer->size) == EXIT_FAILURE) {
                    cominitSensitivePrint("Could not zero out key");
                }
            }
        }
    }

    if (keyBuffer != NULL) {
        if (cominitSecurememoryVerifyZero(keyBuffer->buffer, keyBuffer->size) == EXIT_FAILURE) {
            cominitSensitivePrint("Could not verify that key is zero'ed out");
        }
        munlock(keyBuffer, sizeof(TPM2B_SENSITIVE_DATA));
        Esys_Free(keyBuffer);
        keyBuffer = NULL;
    }

    return state;
}

int cominitSecurememoryCreateLuksVolume(char *devCrypt) {
    int result = EXIT_FAILURE;
    uint8_t *keyBuffer = NULL;
    size_t keyBufferSize = COMINIT_PASSPHRASE_SIZE;

    if (devCrypt == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        keyBuffer = calloc(1, keyBufferSize);
        if (keyBuffer == NULL) {
            cominitErrnoPrint("calloc failed");
        } else {
            if (mlock(keyBuffer, keyBufferSize) != 0) {
                cominitErrnoPrint("mlock failed");
            } else {
                size_t keySize = cominitKeyringGetKey(keyBuffer, keyBufferSize, COMINIT_TPM_SECURE_STORAGE_KEY_NAME);
                if (keySize <= 0) {
                    cominitErrPrint("Could not get passphrase from keyring.");
                } else {
                    cominitSensitivePrint("key length is %d", keySize);
                    result = cominitCryptsetupCreateLuksVolume(devCrypt, keyBuffer, keySize);
                    if (cominitSecurememoryZeroOut(keyBuffer, keyBufferSize) == EXIT_FAILURE) {
                        cominitSensitivePrint("Could not zero out key");
                    }
                }
            }
        }
    }

    if (keyBuffer != NULL) {
        if (cominitSecurememoryVerifyZero(keyBuffer, keyBufferSize) == EXIT_FAILURE) {
            cominitSensitivePrint("Could not verify that key is zero'ed out");
        }
        munlock(keyBuffer, keyBufferSize);
        free(keyBuffer);
        keyBuffer = NULL;
    }

    return result;
}

int cominitSecurememoryEsysCreate(ESYS_CONTEXT *ectx, ESYS_TR *primaryHandle, TPM2B_PUBLIC **outPublic,
                                  TPM2B_PRIVATE **outPrivate, TPM2B_DIGEST *policyDigest) {
    int result = EXIT_FAILURE;
    TPM2B_SENSITIVE_CREATE *keyBuffer = NULL;
    TPM2B_CREATION_DATA *creationData = NULL;
    TPM2B_DIGEST *creationHash = NULL;
    TPMT_TK_CREATION *creationTicket = NULL;

    if (ectx == NULL || primaryHandle == NULL || outPublic == NULL || outPrivate == NULL || policyDigest == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        keyBuffer = calloc(1, sizeof(TPM2B_SENSITIVE_CREATE));
        if (keyBuffer == NULL) {
            cominitErrnoPrint("calloc failed");
        } else {
            if (mlock(keyBuffer, sizeof(TPM2B_SENSITIVE_CREATE)) != 0) {
                cominitErrnoPrint("mlock failed");
            } else {
                TPM2B_PUBLIC inPublic = {
                    .size = 0,
                    .publicArea = {
                        .type = TPM2_ALG_KEYEDHASH,
                        .nameAlg = TPM2_ALG_SHA256,
                        .objectAttributes = (TPMA_OBJECT_USERWITHAUTH | TPMA_OBJECT_FIXEDTPM | TPMA_OBJECT_FIXEDPARENT),

                        .authPolicy =
                            {
                                .size = 0,
                            },
                        .parameters.keyedHashDetail = {.scheme = {.scheme = TPM2_ALG_NULL,
                                                                  .details = {.hmac = {.hashAlg = TPM2_ALG_SHA256}}}},
                        .unique.keyedHash =
                            {
                                .size = 0,
                                .buffer = {0},
                            },
                    }};
                TPM2B_DATA outsideInfo = {
                    .size = 0,
                    .buffer = {0},
                };

                TPML_PCR_SELECTION creationPCR = {
                    .count = 0,
                };
                inPublic.publicArea.authPolicy.size = policyDigest->size;
                memcpy(inPublic.publicArea.authPolicy.buffer, policyDigest->buffer, policyDigest->size);

                keyBuffer->size = sizeof(keyBuffer->sensitive);
                keyBuffer->sensitive.userAuth.size = 0;
                keyBuffer->sensitive.data.size = COMINIT_PASSPHRASE_SIZE;

                result =
                    cominitCryptoCreatePassphrase(keyBuffer->sensitive.data.buffer, keyBuffer->sensitive.data.size);
                if (result != EXIT_SUCCESS) {
                    cominitErrPrint("Could not generate passphrase.");
                } else {
                    TSS2_RC rc = Esys_Create(ectx, *primaryHandle, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE,
                                             keyBuffer, &inPublic, &outsideInfo, &creationPCR, outPrivate, outPublic,
                                             &creationData, &creationHash, &creationTicket);
                    if (rc != TSS2_RC_SUCCESS) {
                        cominitErrPrint("Creation of blob failed");
                    } else {
                        result = EXIT_SUCCESS;
                    }
                    if (cominitSecurememoryZeroOut(keyBuffer->sensitive.data.buffer, keyBuffer->sensitive.data.size) ==
                        EXIT_FAILURE) {
                        cominitSensitivePrint("Could not zero out key");
                    }
                }
            }
        }
    }

    if (keyBuffer != NULL) {
        if (cominitSecurememoryVerifyZero(keyBuffer->sensitive.data.buffer, keyBuffer->sensitive.data.size) ==
            EXIT_FAILURE) {
            cominitSensitivePrint("Could not verify that key is zero'ed out");
        }
        munlock(keyBuffer, sizeof(TPM2B_SENSITIVE_CREATE));
        free(keyBuffer);
        keyBuffer = NULL;
    }

    Esys_Free(creationData);
    Esys_Free(creationHash);
    Esys_Free(creationTicket);

    return result;
}
