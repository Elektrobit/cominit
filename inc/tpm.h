// SPDX-License-Identifier: MIT
/**
 * @file tpm.h
 * @brief Header related to TPM implementations
 */
#ifndef __TPM_H__
#define __TPM_H__

#include <stdint.h>
#include <tss2/tss2_esys.h>
#include <tss2/tss2_tctildr.h>

#include "common.h"

#define COMINIT_TPM_MNT_PT "/tpm"
#define COMINIT_TPM_BLOB_LOCATION "sealed.blob"

/**
 * Structure holding Tpm Context that is acquired during RT.
 */
typedef struct cominitTpmContext {
    ESYS_CONTEXT *esysCtx;       ///< The Pointer to the ESYS context handle returned by Esys_Initialize().
    TSS2_TCTI_CONTEXT *tctiCtx;  ///< The Pointer to the TCTI context handle returned by Tss2_TctiLdr_Initialize().
} cominitTpmContext_t;

/**
 * Result codes for a TPM seal/unseal operation.
 */
typedef enum {
    TpmFailure,        ///< A general failure occurred during the TPM operation.
    TpmPolicyFailure,  ///< Failed because the TPM policy did not authorize the operation (platform state not trusted).
    Unsealed,          ///< Data was successfully unsealed by the TPM.
    Sealed,            ///< Data was successfully sealed by the TPM.
} cominitTpmState_t;

/**
 * Perform TPM-based sealing (on first boot) and unsealing (on every boot) of data.
 *
 * Protected data is only unsealed if the current platform state is trusted.
 * If the TPM policy check fails, the response is handled by cominitTpmHandlePolicyFailure().
 *
 * Called by cominit if its uses TPM.
 *
 * @param tpmCtx   Pointer to the structure that holds the acquired TPM context.
 * @param argCtx   Pointer to the structure that holds the parsed options.
 * @return  Unsealed=2 on success, TpmPolicyFailure=1 or TpmFailure=0 otherwise
 */
cominitTpmState_t cominitTpmProtectData(cominitTpmContext_t *tpmCtx, cominitCliArgs_t *argCtx);

/**
 * Parses the PCR index from argv that should be extended.
 *
 * Called by cominit if its uses TPM.
 *
 * @param argCtx   Pointer to the structure that receives the parsed options.
 * @param argValue  The parsed value of the argument found in the provided argument vector.
 * @return  0 on success, 1 otherwise
 */
int cominitTpmParsePcrIndex(cominitCliArgs_t *argCtx, const char *argValue);

/**
 * Parses a list of PCR indexes from argv to build a policy for sealing data.
 *
 * Called by cominit if its uses TPM.
 *
 * @param argCtx   Pointer to the structure that holds the parsed options.
 * @param argValue  The parsed value of the argument found in the provided argument vector.
 * @return  0 on success, 1 otherwise
 */
int cominitTpmParsePcrIndexes(cominitCliArgs_t *argCtx, const char *argValue);

/**
 * Releases shared run‑time resources that the TPM module
 * acquires during execution.
 *
 * @param tpmCtx   The TPM context.
 * @return  0 on success, 1 otherwise
 */
int cominitDeleteTpm(cominitTpmContext_t *tpmCtx);

/**
 * Extends the PCR by the given signature
 *
 * @param tpmCtx   The TPM context.
 * @param keyfile   The Pointer to the file that contains the public key for
 * @param pcrIndex  The index of the PCR to extend.
 *
 * @return  0 on success, 1 otherwise
 */
int cominitTpmExtendPCR(cominitTpmContext_t *tpmCtx, const char *keyfile, unsigned long pcrIndex);

/**
 * Acquires shared run‑time resources that the TPM module
 * needs during execution.
 *
 * @param tpmCtx   The TPM context.
 * @return  0 on success, 1 otherwise
 */
int cominitInitTpm(cominitTpmContext_t *tpmCtx);

#endif /* __TPM_H__ */
