#include "tpm.h"

#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <unistd.h>

#include "mbedtls/error.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"
#include "mbedtls/version.h"
#include "meta.h"
#include "output.h"

#define DER_BUFFER_SIZE 1600  ///< buffer size to hold RSA‑4k key.
#define SHA256_LEN 32         ///< size of SHA256 digest.
#ifndef TPM2_PERSISTENT_FIRST  ///< Address to store primary handle in TPM.
#define TPM2_PERSISTENT_FIRST ((TPMI_DH_PERSISTENT)0x81000000)
#endif

/**
 * Result codes on checking the current state of the blob partition.
 */
typedef enum {
    BlobIsEmpty,   ///< There is no file at #COMINIT_TPM_BLOB_LOCATION.
    BlobExists,    ///< A file at location #COMINIT_TPM_BLOB_LOCATION can be accessed.
    BlobNotFound,  ///< An error occurred while accessing the file at location #COMINIT_TPM_BLOB_LOCATION.
} cominitBlobState_t;

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
 * @return  0 on success, 1 otherwise
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

/**
 * Create a specified directory and any missing parent directories (no error if it already exists).
 *
 * @param dirPath   The fullpath to the directory that should be created.
 * @param mode   The mode of the new directory.
 *
 * @return  0 on success, 1 otherwise
 */
static int cominitMkdir(const char *dirPath, mode_t mode) {
    int result = EXIT_FAILURE;
    int retVal = 0;
    char slash[] = "/";
    char currentPath[COMINIT_ROOTFS_DEV_PATH_MAX] = {0};
    char pathBuffer[COMINIT_ROOTFS_DEV_PATH_MAX] = {0};
    struct stat statbuf;

    strncpy(pathBuffer, dirPath, sizeof(pathBuffer) - 1);

    char *token = strtok(pathBuffer, slash);
    while (token != NULL) {
        strcat(currentPath, slash);
        strcat(currentPath, token);
        retVal = mkdir(currentPath, mode);
        if (retVal != 0) {
            if (errno != EEXIST) {
                cominitErrPrint("mkdir failed", retVal);
                result = EXIT_FAILURE;
                break;
            }
        }
        token = strtok(NULL, slash);
    }

    retVal = stat(currentPath, &statbuf);
    if (retVal != 0 && !S_ISDIR(statbuf.st_mode)) {
        cominitErrPrint("stat failed", retVal);
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

/**
 * Mounts the partition on which the sealed blob is saved to and checks whether the partition is empty.
 *
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @return  BlobIsEmpty=0 or BlobExists=1 on success, BlobNotFound=3 otherwise
 */
static cominitBlobState_t cominitTpmSetupBlob(cominitCliArgs_t *argCtx) {
    cominitBlobState_t state = BlobNotFound;

    if (argCtx->devNodeBlob[0] != '\0') {
        if (cominitMkdir(COMINIT_TPM_MNT_PT, S_IRWXU) == EXIT_FAILURE) {
            cominitErrPrint("Could not create mount directory /'%s/' for device /'%s/'.", COMINIT_TPM_MNT_PT,
                            argCtx->devNodeBlob);
        } else {
            if (mount(argCtx->devNodeBlob, COMINIT_TPM_MNT_PT, "ext4", MS_NOEXEC, "") != 0) {
                cominitErrPrint("Mount for device /'%s/' failed", argCtx->devNodeBlob);
            } else {
                char fullpath[256];
                snprintf(fullpath, sizeof fullpath, "%s/%s", COMINIT_TPM_MNT_PT, COMINIT_TPM_BLOB_LOCATION);

                if (access(fullpath, F_OK) == 0) {
                    state = BlobExists;
                } else if (errno == ENOENT) {
                    char *dirPath = dirname(fullpath);
                    int result = cominitMkdir(dirPath, S_IRWXU);
                    if (result != EXIT_SUCCESS) {
                        cominitErrPrint("Create directory for blob failed");
                    } else {
                        state = BlobIsEmpty;
                    }
                } else {
                    cominitErrPrint("no valid blob device given");
                }
            }
        }
    }

    return state;
}

/**
 * Creates a key to encrypt/decrypt a secure storage.
 *
 * @param ectx The Pointer to the initialized ESYS_CONTEXT handle.
 * @param key Address of a TPM2B_DIGEST* pointer for allocating a TPM2B_DIGEST structure and populating it
 *                   with random bytes.
 * @return  0 on success, 1 otherwise
 */
static int cominitTpmCreateKey(ESYS_CONTEXT *ectx, TPM2B_DIGEST **key) {
    int result = EXIT_FAILURE;
    TSS2_RC rc = Esys_GetRandom(ectx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, SHA256_LEN, key);

    if (rc == TSS2_RC_SUCCESS && *key != NULL) {
        result = EXIT_SUCCESS;
    }

    return result;
}

/**
 * Deleting the key to encrypt/decrypt a secure storage in memory.
 *
 * @param key Pointer to a TPM2B_DIGEST structure holding a key.
 * @return  0 on success, 1 otherwise
 */
static void cominitTpmDeleteKey(TPM2B_DIGEST **key) {
    if (key && *key) {
        memset((*key)->buffer, 0, (*key)->size);
        Esys_Free(*key);
        *key = NULL;
    }
}

/**
 * Saves the sealed blob to the given path on the blob partition.
 *
 * @param outPublic     The Pointer to the structure that holds the public meta data.
 * @param outPrivate    The Pointer to the structure that holds the private data.
 * @return  0 on success, 1 otherwise
 */
static int cominitTpmSaveBlob(TPM2B_PUBLIC *outPublic, TPM2B_PRIVATE *outPrivate) {
    int result = EXIT_FAILURE;
    FILE *fp = NULL;

    fp = fopen(COMINIT_TPM_MNT_PT "/" COMINIT_TPM_BLOB_LOCATION, "wb");
    if (fp) {
        fwrite(outPublic, 1, sizeof *outPublic, fp);
        fwrite(outPrivate, 1, sizeof *outPrivate, fp);
        result = EXIT_SUCCESS;
    }

    if (fp) {
        fclose(fp);
    }

    return result;
}

/**
 * Makes the primary key persistent on TPM.
 *
 * @param ectx The Pointer to the initialized ESYS_CONTEXT handle.
 * @param primaryHandle Pointer to an ESYS_TR holding a transient primary key handle.
 * @return  0 on success, 1 otherwise
 */
static int cominitTpmSavePrimaryHandle(ESYS_CONTEXT *ectx, ESYS_TR *primaryHandle) {
    int result = EXIT_FAILURE;
    ESYS_TR savedHandle = ESYS_TR_NONE;

    TSS2_RC rc = Esys_EvictControl(ectx, ESYS_TR_RH_OWNER, *primaryHandle, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE,
                                   TPM2_PERSISTENT_FIRST, &savedHandle);

    if (rc != TSS2_RC_SUCCESS) {
        cominitErrPrint("could not save handle");
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

/**
 * Select the PCR that are used for building a policy.
 *
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @param pcrSelection Pointer to a TPML_PCR_SELECTION structure that receives the selected PCR register from the parsed
 * options.
 */
static void cominitTpmSelectPcr(cominitCliArgs_t *argCtx, TPML_PCR_SELECTION *pcrSelection) {
    unsigned long pcrIndex = 0;

    for (int i = 0; i < argCtx->pcrSealCount; i++) {
        pcrIndex = argCtx->pcrSeal[i];
        pcrSelection->pcrSelections[0].pcrSelect[pcrIndex / 8] |= (1u << (pcrIndex % 8));
    }
}

/**
 * Seals a key to a blob and save the used primary key.
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param outPublic Address of a TPM2B_PUBLIC pointer that receives the public meta data.
 * @param outPrivate    Address of a TPM2B_PRIVATE pointer that receives the private data.
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @param key Pointer to an TPM2B_DIGEST structure holding a key.
 * @return  0 on success, 1 otherwise
 */
static int cominitTpmSeal(ESYS_CONTEXT *ectx, TPM2B_PUBLIC **outPublic, TPM2B_PRIVATE **outPrivate,
                          cominitCliArgs_t *argCtx, TPM2B_DIGEST *key) {
    int result = EXIT_FAILURE;
    TPM2B_DIGEST *policyDigest = NULL;
    ESYS_TR sess;
    ESYS_TR primaryHandle = ESYS_TR_NONE;
    TPM2B_PUBLIC *outPublicPrimary = NULL;

    TPM2B_CREATION_DATA *creationData = NULL;
    TPM2B_DIGEST *creationHash = NULL;
    TPMT_TK_CREATION *creationTicket = NULL;
    TPM2B_CREATION_DATA *creationData2 = NULL;
    TPM2B_DIGEST *creationHash2 = NULL;
    TPMT_TK_CREATION *creationTicket2 = NULL;

    TPM2B_SENSITIVE_CREATE inSensitivePrimary = {.size = 0};

    TPM2B_PUBLIC inPublic = {
        .size = 0,
        .publicArea =
            {
                .type = TPM2_ALG_RSA,
                .nameAlg = TPM2_ALG_SHA256,
                .objectAttributes = (TPMA_OBJECT_USERWITHAUTH | TPMA_OBJECT_RESTRICTED | TPMA_OBJECT_DECRYPT |
                                     TPMA_OBJECT_FIXEDTPM | TPMA_OBJECT_FIXEDPARENT | TPMA_OBJECT_SENSITIVEDATAORIGIN),
                .authPolicy =
                    {
                        .size = 0,
                    },
                .parameters.rsaDetail =
                    {
                        .symmetric = {.algorithm = TPM2_ALG_AES, .keyBits.aes = 128, .mode.aes = TPM2_ALG_CFB},
                        .scheme = {.scheme = TPM2_ALG_NULL},
                        .keyBits = 2048,
                        .exponent = 0,
                    },
                .unique.rsa =
                    {
                        .size = 0,
                        .buffer = {0},
                    },
            },
    };

    TPM2B_DATA outsideInfo = {
        .size = 0,
        .buffer = {0},
    };

    TPML_PCR_SELECTION creationPCR = {
        .count = 0,
    };

    TSS2_RC rc = Esys_CreatePrimary(ectx, ESYS_TR_RH_OWNER, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE,
                                    &inSensitivePrimary, &inPublic, &outsideInfo, &creationPCR, &primaryHandle,
                                    &outPublicPrimary, &creationData, &creationHash, &creationTicket);

    if (rc != TSS2_RC_SUCCESS) {
        cominitErrPrint("Create primary failed");
    } else {
        TPMT_SYM_DEF symmetric = {.algorithm = TPM2_ALG_NULL};

        rc = Esys_StartAuthSession(ectx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, NULL,
                                   TPM2_SE_TRIAL, &symmetric, TPM2_ALG_SHA256, &sess);

        if (rc != TSS2_RC_SUCCESS) {
            cominitErrPrint("Start Session failed");
        } else {
            TPML_PCR_SELECTION psel = {
                .count = 1, .pcrSelections = {{.hash = TPM2_ALG_SHA256, .sizeofSelect = 3, .pcrSelect = {0, 0, 0}}}};
            cominitTpmSelectPcr(argCtx, &psel);

            rc = Esys_PolicyPCR(ectx, sess, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, NULL, &psel);

            if (rc != TSS2_RC_SUCCESS) {
                cominitErrPrint("Create policy failed");
            } else {
                rc = Esys_PolicyGetDigest(ectx, sess, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, &policyDigest);
                if (rc != TSS2_RC_SUCCESS) {
                    cominitErrPrint("Get policy digest failed");
                } else {
                    TPM2B_PUBLIC inPublic2 = {
                        .size = 0,
                        .publicArea = {
                            .type = TPM2_ALG_KEYEDHASH,
                            .nameAlg = TPM2_ALG_SHA256,
                            .objectAttributes =
                                (TPMA_OBJECT_USERWITHAUTH | TPMA_OBJECT_FIXEDTPM | TPMA_OBJECT_FIXEDPARENT),

                            .authPolicy =
                                {
                                    .size = 0,
                                },
                            .parameters.keyedHashDetail = {.scheme = {.scheme = TPM2_ALG_NULL,
                                                                      .details = {.hmac = {.hashAlg =
                                                                                               TPM2_ALG_SHA256}}}},
                            .unique.keyedHash =
                                {
                                    .size = 0,
                                    .buffer = {0},
                                },
                        }};

                    inPublic2.publicArea.authPolicy.size = policyDigest->size;
                    memcpy(inPublic2.publicArea.authPolicy.buffer, policyDigest->buffer, policyDigest->size);

                    TPM2B_DATA outsideInfo2 = {
                        .size = 0,
                        .buffer = {0},
                    };

                    TPML_PCR_SELECTION creationPCR2 = {
                        .count = 0,
                    };

                    TPM2B_SENSITIVE_CREATE inSensitive2 = {.size = 0};

                    inSensitive2.sensitive.data.size = key->size;
                    memcpy(inSensitive2.sensitive.data.buffer, key->buffer, key->size);

                    rc = Esys_Create(ectx, primaryHandle, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE, &inSensitive2,
                                     &inPublic2, &outsideInfo2, &creationPCR2, outPrivate, outPublic, &creationData2,
                                     &creationHash2, &creationTicket2);

                    if (rc != TSS2_RC_SUCCESS) {
                        cominitErrPrint("Creation of blob failed");
                    } else {
                        result = cominitTpmSavePrimaryHandle(ectx, &primaryHandle);
                    }
                }
            }
        }
    }

    Esys_FlushContext(ectx, sess);
    Esys_FlushContext(ectx, primaryHandle);
    Esys_Free(creationData);
    Esys_Free(creationHash);
    Esys_Free(creationTicket);
    Esys_Free(creationData2);
    Esys_Free(creationHash2);
    Esys_Free(creationTicket2);
    Esys_Free(policyDigest);
    Esys_Free(outPublicPrimary);

    return result;
}
/**
 * Seals the key into blob and saves it.
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @param key Pointer to an TPM2B_DIGEST structure holding a key.
 * @return  Sealed=3 on success, TpmFailure=0 otherwise
 */
static cominitTpmState_t cominitTpmSealBlob(ESYS_CONTEXT *ectx, cominitCliArgs_t *argCtx, TPM2B_DIGEST *key) {
    TPM2B_PUBLIC *outPublic = NULL;
    TPM2B_PRIVATE *outPrivate = NULL;
    cominitTpmState_t state = TpmFailure;
    int result = EXIT_FAILURE;

    result = cominitTpmSeal(ectx, &outPublic, &outPrivate, argCtx, key);
    if (result != EXIT_SUCCESS) {
        cominitErrPrint("Could not seal the data.");
    } else {
        result = cominitTpmSaveBlob(outPublic, outPrivate);
        if (result != EXIT_SUCCESS) {
            cominitErrPrint("Could not save the sealed blob.");
        }
    }

    if (result == EXIT_SUCCESS) {
        cominitInfoPrint("Key sealed to blob");
        state = Sealed;
    }

    Esys_Free(outPublic);
    Esys_Free(outPrivate);

    return state;
}

/**
 * Unmount the blob partition.
 *
 */
static void cominitTpmUnmountBlob() {
    umount(COMINIT_TPM_MNT_PT);
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
#if MBEDTLS_VERSION_MAJOR == 2
                err = mbedtls_sha256_ret(pubKeyDer, (size_t)derLen, digest, 0);
#else
                err = mbedtls_sha256(pubKeyDer, (size_t)derLen, digest, 0);
#endif
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

int cominitTpmParsePcrIndex(cominitCliArgs_t *argCtx, const char *argValue) {
    int result = EXIT_FAILURE;

    if (argCtx == NULL || argValue == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        argCtx->pcrSet = false;
        errno = 0;
        char *end;
        unsigned long pcrIndex = strtoul(argValue, &end, 0);
        if (!errno && *end == '\0' && pcrIndex < TPM2_PT_PCR_COUNT) {
            result = EXIT_SUCCESS;
            argCtx->pcrSet = true;
            argCtx->pcrIndex = pcrIndex;
        }
    }

    return result;
}

int cominitTpmParsePcrIndexes(cominitCliArgs_t *argCtx, const char *argValue) {
    int result = EXIT_FAILURE;
    unsigned long i = 0;

    if (argCtx == NULL || argValue == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        const char *p = argValue;
        char *end;
        while (*p) {
            errno = 0;
            if (isdigit((unsigned char)*p)) {
                unsigned long pcrIndex = strtoul(p, &end, 10);
                if (errno == 0 && end != p && pcrIndex < TPM2_PT_PCR_COUNT && ARRAY_SIZE(argCtx->pcrSeal) > i) {
                    argCtx->pcrSeal[i++] = pcrIndex;
                    p = end;
                } else {
                    i = 0;
                    break;
                }
            } else {
                p++;
            }
        }
    }

    argCtx->pcrSealCount = i;
    if (i > 0) {
        result = EXIT_SUCCESS;
    }

    return result;
}

cominitTpmState_t cominitTpmProtectData(cominitTpmContext_t *tpmCtx, cominitCliArgs_t *argCtx) {
    cominitBlobState_t blobState = BlobNotFound;
    cominitTpmState_t tpmState = TpmFailure;
    TPM2B_DIGEST *keyToSeal = NULL;

    blobState = cominitTpmSetupBlob(argCtx);

    switch (blobState) {
        case BlobIsEmpty:
            cominitInfoPrint("Blob is empty: sealing");
            int result = cominitTpmCreateKey(tpmCtx->esysCtx, &keyToSeal);
            if (result != EXIT_SUCCESS) {
                cominitErrPrint("Could not generate key.");
            } else {
                tpmState = cominitTpmSealBlob(tpmCtx->esysCtx, argCtx, keyToSeal);
                if (tpmState == Sealed) {
                    cominitTpmDeleteKey(&keyToSeal);
                }
            }
            break;
        case BlobExists:
            cominitInfoPrint("Blob exists: unsealing");
            break;
        case BlobNotFound:
        default:
            cominitInfoPrint("Blob not found");
            tpmState = TpmFailure;
            break;
    }

    cominitTpmDeleteKey(&keyToSeal);

    cominitTpmUnmountBlob(argCtx->devNodeBlob);

    return tpmState;
}
