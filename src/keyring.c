// SPDX-License-Identifier: MIT
/**
 * @file keyring.c
 * @brief Implementation of functions to interact with the Kernel Key Retention Service.
 */
#include "keyring.h"

#include <linux/keyctl.h>
#include <linux/limits.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <unistd.h>

#include "output.h"

/**
 * Convenience wrapper macro for the read operation of the keyctl system call.
 *
 * See https://man7.org/linux/man-pages/man2/keyctl.2.html
 *
 * @param keyID  They key ID to read the payload from (int32_t).
 * @param pld    Return pointer where the payload gets written (char *).
 * @param pldSz  Size of the payload buffer (size_t).
 *
 * @return  The length of the key's payload on success, -1 otherwise.
 */
#define cominitKeyctlRead(keyID, pld, pldSz) syscall(SYS_keyctl, KEYCTL_READ, keyID, pld, pldSz, 0)
/**
 * Convenience wrapper macro for the search operation of the keyctl system call.
 *
 * See https://man7.org/linux/man-pages/man2/keyctl.2.html
 *
 * @param keyringID  ID of the keyring to search in (int32_t). Search is recursive for all nested keyrings with `search`
 *                   permission.
 * @param keyType    The type of the key to search for (char *, see man page).
 * @param keyDesc    The description string of the key to search for (char *).
 *
 * @return  The key's ID (as int32_t) on success, -1 otherwise.
 */
#define cominitKeyctlSearch(keyringID, keyType, keyDesc) \
    syscall(SYS_keyctl, KEYCTL_SEARCH, keyringID, keyType, keyDesc, 0)

ssize_t cominitGetKey(uint8_t *key, size_t keyMaxLen, char *keyDesc) {
    if (key == NULL || keyDesc == NULL) {
        cominitErrPrint("Parameters must not be NULL.");
        return -1;
    }
    if (strnlen(keyDesc, 4095) == 4095) {
        cominitErrPrint(
            "The given key description string is longer than the maximum length accepted by the Kernel keyring.");
        return -1;
    }
    int32_t keyID = cominitKeyctlSearch(KEY_SPEC_USER_KEYRING, "user", keyDesc);
    if (keyID == -1) {
        cominitErrnoPrint("Could not find key \'%s\' in Kernel user keyring.", keyDesc);
        return -1;
    }
    ssize_t len = cominitKeyctlRead(keyID, key, keyMaxLen);
    if (len == -1) {
        cominitErrnoPrint("Could not read key payload of key \'%s\' from Kernel user keyring.", keyDesc);
        return -1;
    }
    return len;
}

#ifdef COMINIT_FAKE_HSM
int cominitInitFakeHsm(void) {
    char *runner, *strtokState;
    char keyDescs[sizeof(COMINIT_FAKE_HSM_KEY_DESCS)];
    char keyPath[PATH_MAX];
    uint8_t keyPld[COMINIT_KEYRING_PAYLOAD_MAX_SIZE];
    strcpy(keyDescs, COMINIT_FAKE_HSM_KEY_DESCS);

    runner = strtok_r(keyDescs, " ", &strtokState);
    while (runner != NULL) {
        if (snprintf(keyPath, sizeof(keyPath), COMINIT_FAKE_HSM_KEY_DIR "/%s.bin", runner) < 0) {
            cominitErrPrint("Could not format path for key \'%s\'.", runner);
            return -1;
        }
        struct stat keyfileInfo;
        if (stat(keyPath, &keyfileInfo) == -1) {
            cominitErrnoPrint("Could not stat %s.", keyPath);
            return -1;
        }
        if (keyfileInfo.st_size > COMINIT_KEYRING_PAYLOAD_MAX_SIZE) {
            cominitErrPrint("Given keyfile \'%s\' is larger (%d Bytes) than the maximum allowed size (%d Bytes).",
                            keyPath, keyfileInfo.st_size, COMINIT_KEYRING_PAYLOAD_MAX_SIZE);
            return -1;
        }
        FILE *keyFp = fopen(keyPath, "rb");
        if (keyFp == NULL) {
            cominitErrnoPrint("Could not open \'%s\' for reading.", keyPath);
            return -1;
        }
        if (fread(keyPld, keyfileInfo.st_size, 1, keyFp) != 1) {
            cominitErrPrint("Tried to read %d Bytes from \'%s\' and failed.", keyfileInfo.st_size, keyPath);
            return -1;
        }

        if (syscall(SYS_add_key, "user", runner, keyPld, keyfileInfo.st_size, KEY_SPEC_USER_KEYRING) == -1) {
            cominitErrnoPrint("Could not add key \'%s\' of size %d Bytes to user keyring.", runner,
                              keyfileInfo.st_size);
            return -1;
        }

        cominitInfoPrint("Added key \'%s\' to keyring.", runner);
        runner = strtok_r(NULL, " ", &strtokState);
    }
    return 0;
}
#endif
