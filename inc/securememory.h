// SPDX-License-Identifier: MIT
/**
 * @file securememory.h
 * @brief Header related to securememory implementations
 */

#ifndef __SECUREMEMORY_H__
#define __SECUREMEMORY_H__

#include <stddef.h>

#include "tpm.h"

/**
 * Creates a new LUKS2 volume on the target device by
 * retrieving the passphrase from user keyring and
 * calling cominitCryptsetupCreateLuksVolume().
 *
 * Use single buffer for passphrase and zeroes it out with
 * cominitSecurememoryZeroOut().
 *
 * @param devCrypt The target device.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitSecurememoryCreateLuksVolume(char *devCrypt);

/**
 * Unseals the passphrase from TPM sealed blob and adds it
 * to user keyring.
 *
 * Use single buffer for passphrase and zeroes it out with
 * cominitSecurememoryZeroOut().
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param primaryHandle  Pointer to a ESYS_TR structure that holds the primary key handle.
 * @param outPublic Address of a TPM2B_PUBLIC pointer that receives the public meta data.
 * @param outPrivate    Address of a TPM2B_PRIVATE pointer that receives the private data.
 * @param policyDigest   Pointer to a TPM2B_DIGEST structure that holds policy digest.
 *
 * @return  Unsealed=2 on success, TpmPolicyFailure=1 or TpmFailure=0 otherwise
 */
int cominitSecurememoryEsysCreate(ESYS_CONTEXT *ectx, ESYS_TR *primaryHandle, TPM2B_PUBLIC **outPublic,
                                  TPM2B_PRIVATE **outPrivate, TPM2B_DIGEST *policyDigest);
/**
 * Unseals the passphrase from TPM sealed blob and adds it
 * to user keyring.
 *
 * Use single buffer for passphrase and zeroes it out with
 * cominitSecurememoryZeroOut().
 *
 * @param ectx  The Pointer to the initialized ESYS_CONTEXT handle.
 * @param blobHandle  Pointer to a ESYS_TR structure that holds the blob key handle.
 * @param session   Pointer to a ESYS_TR structure that holds the session handle.
 *
 * @return  Unsealed=2 on success, TpmPolicyFailure=1 or TpmFailure=0 otherwise
 */
cominitTpmState_t cominitSecurememoryEsysUnseal(ESYS_CONTEXT *ectx, ESYS_TR *blobHandle, ESYS_TR *session);

#endif /* __SECUREMEMORY_H__ */