#include "cryptsetup.h"

#include <stdio.h>

#include "output.h"
#include "subprocess.h"
#include "tpm.h"

#ifndef COMINIT_CRYPTSETUP_DIR
#define COMINIT_CRYPTSETUP_DIR "/usr/sbin/cryptsetup"
#endif

int cominitCryptsetupCreateLuksVolume(char *devCrypt, uint8_t *passphrase, size_t passphraseSize) {
    int result = EXIT_FAILURE;

    if (devCrypt == NULL || passphrase == NULL || passphraseSize <= 0) {
        cominitErrPrint("Invalid parameters");
    } else {
        char *const argv[] = {(char *)COMINIT_CRYPTSETUP_DIR,
                              "luksFormat",
                              "--batch-mode",
                              "--type",
                              "luks2",
                              "--key-size",
                              "512",
                              "--cipher",
                              "aes-xts-plain64",
                              "--iter-time",
                              "10",
                              "--key-file",
                              "-",
                              devCrypt,
                              NULL};
        char *env[] = {NULL};

        result = cominitSubprocessSpawnAndWrite(argv[0], argv, env, passphrase, passphraseSize);
    }

    return result;
}

int cominitCryptsetupOpenLuksVolume(char *devCrypt) {
    int result = EXIT_FAILURE;

    if (devCrypt == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        char *const argv[] = {(char *)COMINIT_CRYPTSETUP_DIR, "luksOpen", devCrypt,
                              (char *)COMINIT_TPM_SECURE_STORAGE_NAME, NULL};
        char *env[] = {NULL};

        result = cominitSubprocessSpawn(argv[0], argv, env);
    }

    return result;
}

int cominitCryptsetupAddToken(char *devCrypt) {
    int result = EXIT_FAILURE;

    if (devCrypt == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        char *const argv[] = {(char *)COMINIT_CRYPTSETUP_DIR,
                              "token",
                              "add",
                              "--token-type",
                              "keyring",
                              "--key-slot",
                              "0",
                              "--key-description",
                              (char *)COMINIT_TPM_SECURE_STORAGE_KEY_NAME,
                              (char *)devCrypt,
                              NULL};
        char *env[] = {NULL};

        result = cominitSubprocessSpawn(argv[0], argv, env);
    }

    return result;
}
