// SPDX-License-Identifier: MIT
/**
 * @file keyring.h
 * @brief Header related to interactions with the Kernel Key Retention Service.
 */
#ifndef __KEYRING_H__
#define __KEYRING_H__

#include <stdint.h>
#include <sys/types.h>

/**
 * Maximum size (in Bytes) we want to allow for a single key.
 */
#define COMINIT_KEYRING_PAYLOAD_MAX_SIZE 512

/**
 * Searches for a key according to \a keyDesc in the user keyring (UID 0 in this case) and returns it in \a key if
 * found.
 *
 * @param key        Return pointer for the key's payload. Must have enough space for \a keyMaxLen elements.
 * @param keyMaxLen  Maximum length of payload to return.
 * @param keyDesc    Null-terminated description string of the key which will be used to search for it.
 *
 * @return  The size of the returned payload on success. -1 if no key was found or another error occurred.
 */
ssize_t cominitKeyringGetKey(uint8_t *key, size_t keyMaxLen, char *keyDesc);

/**
 * Adds a key according to \a keyDesc in the user keyring (UID 0 in this case).
 *
 * @param key        Pointer to the key's payload.
 * @param keyLen     Length of the key.
 * @param keyDesc    Null-terminated description string of the key.
 *
 * @return  0 on success, -1 otherwise
 */
int cominitKeyringAddUserKey(const char *keyDesc, const uint8_t *key, size_t keyLen);

#ifdef COMINIT_FAKE_HSM

#ifndef COMINIT_FAKE_HSM_KEY_DIR
/**
 * The directory in initramfs where to look for the keyfiles.
 *
 * Can be externally supplied via the FAKE_HSM_KEY_DIR cmake option.
 */
#define COMINIT_FAKE_HSM_KEY_DIR "/etc/fake_hsm"
#endif

#ifndef COMINIT_FAKE_HSM_KEY_DESCS
/**
 * The description fields (names) of the keys to be enrolled on boot if COMINIT_FAKE_HSM is also defined.
 *
 * Format should be a space-delimited string like `"<key_desc_1> <key_desc_2> ... <key_desc_N>"` Key files are expected
 * to be accessible to `cominit` at #COMINIT_FAKE_HSM_KEY_DIR`/<key_desc_n>.bin`.
 *
 * Can be externally supplied via the FAKE_HSM_KEY_DESCS cmake option.
 */
#define COMINIT_FAKE_HSM_KEY_DESCS "dm-integrity-hmac-secret dm-integrity-jmac-secret dm-integrity-jcrypt-secret"
#endif

/**
 * Loads keys to the keyring for development.
 *
 * Needs to be activated at compile-time using -DCOMINIT_FAKE_HSM. This is intended for development only. In production
 * use, necessary keys are expected to be enrolled in the Kernel keyring through secure means on boot, e.g. through a
 * properly set up hardware security module (HSM). During development, this may be unavailable for various reasons. So
 * this function essentially emulates this interface if called before any other functions which depend on a key.
 *
 * It will enroll all keys defined in #COMINIT_KEYRING_KEY_DESCS. As described there, the key files must be accessible
 * to cominit in the initramfs.
 *
 * @return  0 on success, -1 on failure.
 */
int cominitKeyringInitFakeHsm(void);
#endif

#endif /* __KEYRING_H__ */
