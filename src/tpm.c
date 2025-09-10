// SPDX-License-Identifier: MIT
/**
 * @file tpm.c
 * @brief Implementation related to TPM handling
 */

#include "tpm.h"

#include <ctype.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

#include "crypto.h"
#include "cryptsetup.h"
#include "dmctl.h"
#include "keyring.h"
#include "meta.h"
#include "output.h"
#include "securememory.h"
#include "subprocess.h"

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
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
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
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
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
 * Saves the sealed blob to the given path on the blob partition.
 *
 * @param outPublic     The Pointer to the structure that holds the public meta data.
 * @param outPrivate    The Pointer to the structure that holds the private data.
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
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
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
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
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitTpmSeal(ESYS_CONTEXT *ectx, TPM2B_PUBLIC **outPublic, TPM2B_PRIVATE **outPrivate,
                          cominitCliArgs_t *argCtx) {
    int result = EXIT_FAILURE;
    TPM2B_DIGEST *policyDigest = NULL;
    ESYS_TR sess;
    ESYS_TR primaryHandle = ESYS_TR_NONE;
    TPM2B_PUBLIC *outPublicPrimary = NULL;

    TPM2B_CREATION_DATA *creationData = NULL;
    TPM2B_DIGEST *creationHash = NULL;
    TPMT_TK_CREATION *creationTicket = NULL;

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
                    result = cominitSecurememoryEsysCreate(ectx, &primaryHandle, outPublic, outPrivate, policyDigest);
                    if (rc != EXIT_SUCCESS) {
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
    Esys_Free(policyDigest);
    Esys_Free(outPublicPrimary);

    return result;
}

/**
 * Formats the secure storage partition to ext4 on first boot.
 *
 * */
static int cominitTpmFormatSecureStorage() {
    char *argv[] = {"/sbin/mkfs.ext4", "-F", (char *)COMINIT_TPM_SECURE_STORAGE_LOCATION, NULL};
    char *env[] = {NULL};
    return cominitSubprocessSpawn(argv[0], argv, env);
}

/**
 * Sets up the secure storage with the unsealed key.
 *
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @param isFirstBoot Flag to indicate whether it is the very first boot.
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitTpmSetupSecureStorage(cominitCliArgs_t *argCtx, bool isFirstBoot) {
    int result = EXIT_FAILURE;

    if (isFirstBoot == true) {
        result = cominitSecurememoryCreateLuksVolume(argCtx->devNodeCrypt);
        if (result != EXIT_SUCCESS) {
            cominitErrPrint("Could not create LUKS volume");
        } else {
            result = cominitCryptsetupAddToken(argCtx->devNodeCrypt);
            if (result != EXIT_SUCCESS) {
                cominitErrPrint("Could not add LUKS token");
            } else {
                result = cominitCryptsetupOpenLuksVolume(argCtx->devNodeCrypt);
                if (result != EXIT_SUCCESS) {
                    cominitErrPrint("Could not open LUKS volume");
                } else {
                    result = cominitTpmFormatSecureStorage();
                    if (result != EXIT_SUCCESS) {
                        cominitErrPrint("formating secure storage failed");
                    }
                }
            }
        }
    }

    else {
        result = cominitCryptsetupOpenLuksVolume(argCtx->devNodeCrypt);
        if (result != EXIT_SUCCESS) {
            cominitErrPrint("Could not open LUKS volume");
        }
    }

    return result;
}

/**
 * Seals the key into blob and saves it.
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @return  Sealed=3 on success, TpmFailure=0 otherwise
 */
static cominitTpmState_t cominitTpmSealBlob(ESYS_CONTEXT *ectx, cominitCliArgs_t *argCtx) {
    TPM2B_PUBLIC *outPublic = NULL;
    TPM2B_PRIVATE *outPrivate = NULL;
    cominitTpmState_t state = TpmFailure;
    int result = EXIT_FAILURE;

    result = cominitTpmSeal(ectx, &outPublic, &outPrivate, argCtx);
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

/**
 * Loads the primary key for unsealing.
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param primaryHandle Pointer to a ESYS_TR structure that receives the primary key handle.
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitTpmLoadPrimaryHandle(ESYS_CONTEXT *ectx, ESYS_TR *primaryHandle) {
    int result = EXIT_FAILURE;

    TSS2_RC rc =
        Esys_TR_FromTPMPublic(ectx, TPM2_PERSISTENT_FIRST, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, primaryHandle);

    if (rc != TSS2_RC_SUCCESS) {
        cominitErrPrint("Could not open primary handle");
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

/**
 * Loads the sealed blob from a given path on the blob partition.
 *
 * @param outPublic The Pointer to the structure that receives the public meta data.
 * @param outPrivate    The Pointer to the structure that receives the private data.
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitTpmLoadBlob(TPM2B_PUBLIC *outPublic, TPM2B_PRIVATE *outPrivate) {
    int result = EXIT_FAILURE;
    FILE *fp = NULL;

    fp = fopen(COMINIT_TPM_MNT_PT "/" COMINIT_TPM_BLOB_LOCATION, "rb");
    if (fp) {
        size_t want = sizeof *outPublic;
        if (fread(outPublic, 1, want, fp) == want) {
            want = sizeof *outPrivate;
            if (fread(outPrivate, 1, want, fp) == want) {
                result = EXIT_SUCCESS;
            }
        }
    }

    if (fp) {
        fclose(fp);
    }

    return result;
}

/**
 * Unseals the key.
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param primaryHandle Pointer to an ESYS_TR holding a transient primary key handle.
 * @param outPublic The Pointer to the structure that holds the public meta data.
 * @param outPrivate    The Pointer to the structure that holds the private data.
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @return  Unsealed=2 on success, TpmPolicyFailure=1 or TpmFailure=0 otherwise
 */
static cominitTpmState_t cominitTpmUnseal(ESYS_CONTEXT *ectx, ESYS_TR *primaryHandle, TPM2B_PUBLIC *outPublic,
                                          TPM2B_PRIVATE *outPrivate, cominitCliArgs_t *argCtx) {
    ESYS_TR blobHandle = ESYS_TR_NONE;
    ESYS_TR sess = ESYS_TR_NONE;
    cominitTpmState_t state = TpmFailure;

    TSS2_RC rc = Esys_Load(ectx, *primaryHandle, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE, outPrivate, outPublic,
                           &blobHandle);

    if (rc != TSS2_RC_SUCCESS) {
        cominitErrPrint("Load of handle failed");
    } else {
        TPMT_SYM_DEF symmetric = {.algorithm = TPM2_ALG_NULL};
        rc = Esys_StartAuthSession(ectx, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, NULL,
                                   TPM2_SE_POLICY, &symmetric, TPM2_ALG_SHA256, &sess);
        if (rc != TSS2_RC_SUCCESS) {
            cominitErrPrint("Starting session failed");
        } else {
            TPML_PCR_SELECTION psel = {
                .count = 1, .pcrSelections = {{.hash = TPM2_ALG_SHA256, .sizeofSelect = 3, .pcrSelect = {0, 0, 0}}}};
            cominitTpmSelectPcr(argCtx, &psel);

            rc = Esys_PolicyPCR(ectx, sess, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, NULL, &psel);

            if (rc != TSS2_RC_SUCCESS) {
                cominitErrPrint("Creating policy failed");
            } else {
                state = cominitSecurememoryEsysUnseal(ectx, &blobHandle, &sess);
            }
        }
    }

    Esys_FlushContext(ectx, sess);
    Esys_FlushContext(ectx, blobHandle);

    return state;
}

/**
 * Loads the primary key and the private data to unseal the key.
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param argCtx Pointer to the structure that holds the parsed options.
 * @return  Unsealed=2 on success, TpmPolicyFailure=1 or TpmFailure=0 otherwise
 */
static cominitTpmState_t cominitTpmUnsealBlob(ESYS_CONTEXT *ectx, cominitCliArgs_t *argCtx) {
    TPM2B_PUBLIC outPublic = {0};
    TPM2B_PRIVATE outPrivate = {0};
    ESYS_TR primaryHandle = ESYS_TR_NONE;
    int result = EXIT_FAILURE;
    cominitTpmState_t tpmState = TpmFailure;

    result = cominitTpmLoadPrimaryHandle(ectx, &primaryHandle);

    if (result != EXIT_SUCCESS) {
        cominitErrPrint("Could not retrieve handle.");
    } else {
        result = cominitTpmLoadBlob(&outPublic, &outPrivate);
        if (result != EXIT_SUCCESS) {
            cominitErrPrint("Could not retrieve blob.");
        } else {
            tpmState = cominitTpmUnseal(ectx, &primaryHandle, &outPublic, &outPrivate, argCtx);
        }
    }

    return tpmState;
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
    unsigned char digest[SHA256_LEN];

    if (tpmCtx == NULL || keyfile == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        result = cominitCreateSHA256DigestfromKeyfile(keyfile, digest, ARRAY_SIZE(digest));
        if (result != EXIT_SUCCESS) {
            cominitErrPrint("Could not hash the rootfs public key");
        } else {
            ESYS_TR pcrTR = ESYS_TR_PCR0 + pcrIndex;
            TPML_DIGEST_VALUES vals = {.count = 1};
            vals.digests[0].hashAlg = TPM2_ALG_SHA256;
            memcpy(vals.digests[0].digest.sha256, digest, sizeof(digest));

            TSS2_RC rc = Esys_PCR_Extend(tpmCtx->esysCtx, pcrTR, ESYS_TR_PASSWORD, ESYS_TR_NONE, ESYS_TR_NONE, &vals);
            if (rc != TSS2_RC_SUCCESS) {
                result = EXIT_FAILURE;
            }
        }
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

int cominitTpmMountSecureStorage() {
    int result = cominitMkdir(COMINIT_TPM_SECURE_STORAGE_MNT, S_IRWXU);
    if (result != EXIT_SUCCESS) {
        cominitErrPrint("creating mount point for secure storage failed");
    } else {
        if (mount(COMINIT_TPM_SECURE_STORAGE_LOCATION, COMINIT_TPM_SECURE_STORAGE_MNT, "ext4", 0, "") != 0) {
            result = -EXIT_FAILURE;
        } else {
            result = EXIT_SUCCESS;
        }
    }

    return result;
}

int cominitTpmHandlePolicyFailure(cominitTpmContext_t *tpmCtx) {
    int result = EXIT_FAILURE;

    cominitErrPrint("Policy check failed");

    if (tpmCtx == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        result = EXIT_SUCCESS;
    }

    return result;
}

bool cominitTpmSecureStorageEnabled(cominitCliArgs_t *argCtx) {
    bool secureStorageEnabled = false;

    if (argCtx == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        if (argCtx->devNodeBlob[0] != '\0' && argCtx->pcrSealCount > 0 && argCtx->devNodeCrypt[0] != '\0') {
            secureStorageEnabled = true;
        }
    }

    return secureStorageEnabled;
}

bool cominitTpmExtendEnabled(cominitCliArgs_t *argCtx) {
    bool extendEnabled = false;

    if (argCtx == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        if (argCtx->pcrSet == true) {
            extendEnabled = true;
        }
    }

    return extendEnabled;
}

cominitTpmState_t cominitTpmProtectData(cominitTpmContext_t *tpmCtx, cominitCliArgs_t *argCtx) {
    cominitBlobState_t blobState = BlobNotFound;
    cominitTpmState_t tpmState = TpmFailure;

    blobState = cominitTpmSetupBlob(argCtx);

    switch (blobState) {
        case BlobIsEmpty:
            cominitInfoPrint("Blob is empty: sealing");
            tpmState = cominitTpmSealBlob(tpmCtx->esysCtx, argCtx);
            if (tpmState == Sealed) {
                tpmState = cominitTpmUnsealBlob(tpmCtx->esysCtx, argCtx);
            }
            if (tpmState == Unsealed) {
                if (cominitTpmSetupSecureStorage(argCtx, true) != EXIT_SUCCESS) {
                    cominitErrPrint("Secure storage could not be set up.");
                    tpmState = TpmFailure;
                }
            }
            break;
        case BlobExists:
            cominitInfoPrint("Blob exists: unsealing");
            tpmState = cominitTpmUnsealBlob(tpmCtx->esysCtx, argCtx);
            if (tpmState == Unsealed) {
                if (cominitTpmSetupSecureStorage(argCtx, false) != EXIT_SUCCESS) {
                    cominitErrPrint("Secure storage could not be set up.");
                    tpmState = TpmFailure;
                }
            }
            break;
        case BlobNotFound:
        default:
            cominitInfoPrint("Blob not found");
            tpmState = TpmFailure;
            break;
    }

    cominitTpmUnmountBlob(argCtx->devNodeBlob);

    return tpmState;
}
