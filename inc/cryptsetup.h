// SPDX-License-Identifier: MIT
/**
 * @file cryptsetup.h
 * @brief Header related to cryptsetup implementations
 */

#ifndef __CRYPTSETUP_H__
#define __CRYPTSETUP_H__

#include <tss2/tss2_esys.h>

/**
 * Creates a new LUKS2 volume on the target device using the cryptsetup luksFormat subcommand
 *
 * @param devCrypt      The target device.
 * @param passphrase    Pointer to the buffer containing a passphrase for unlocking a LUKS volume.
 *
 * @return  0 on success, 1 otherwise
 */
int cominitCryptsetupCreateLuksVolume(char *devCrypt, TPM2B_DIGEST *passphrase);

/**
 * Opens a LUKS2 volume on the target device using the cryptsetup luksOpen subcommand.
 *
 * @param devCrypt      The target device.
 * @param passphrase    Pointer to the buffer containing a passphrase for unlocking a LUKS volume.
 *
 * @return  0 on success, 1 otherwise
 */
int cominitCryptsetupOpenLuksVolume(char *devCrypt, TPM2B_DIGEST *passphrase);

#endif /* __CRYPTSETUP_H__ */
