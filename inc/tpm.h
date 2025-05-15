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

/**
 * Structure holding Tpm Context that is acquired during RT.
 */
typedef struct cominitTpmContext {
    ESYS_CONTEXT *esysCtx;       ///< The Pointer to the ESYS context handle returned by Esys_Initialize().
    TSS2_TCTI_CONTEXT *tctiCtx;  ///< The Pointer to the TCTI context handle returned by Tss2_TctiLdr_Initialize().
} cominitTpmContext_t;

/**
 * Parses the PCR index from argv.
 *
 * Called by cominit if its uses TPM.
 *
 * @param ctx   Pointer to the structure that receives the parsed options.
 * @param argc  Number of elements in @p argv.
 * @param argv  The provided argument vector.
 * @param i     Index in @p argv to check for a valid PCR.
 * @return  0 on success, 1 otherwise
 */
int cominitTpmParsePcrIndex(cominitCliArgs_t *ctx, int argc, char **argv, int i);

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
 * @return  0 on success, -1 otherwise
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
