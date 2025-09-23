// SPDX-License-Identifier: MIT
/**
 * @file cryptsetup.h
 * @brief Header related to cryptsetup implementations
 */

#ifndef __CRYPTSETUP_H__
#define __CRYPTSETUP_H__

#include <stddef.h>
#include <stdint.h>

#define COMINIT_PASSPHRASE_SIZE 32  ///< The size of the passphrase.

/**
 * Creates a new LUKS2 volume on the target device using the cryptsetup luksFormat subcommand
 *
 * @param devCrypt      The target device.
 * @param passphrase    Pointer to the buffer containing a passphrase for unlocking a LUKS volume.
 * @param passphraseLen Size of the passphrase.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitCryptsetupCreateLuksVolume(char *devCrypt, uint8_t *passphrase, size_t passphraseLen);

/**
 * Opens a LUKS2 volume on the target device using the cryptsetup luksOpen subcommand.
 * The passphrase is obtained by token from user key ring and must been added previously.
 *
 * @param devCrypt      The target device.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitCryptsetupOpenLuksVolume(char *devCrypt);

/**
 * Adds a token to an existing LUKS volume.
 *
 * @param devCrypt      The target device.
 *
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitCryptsetupAddToken(char *devCrypt);

#endif /* __CRYPTSETUP_H__ */
