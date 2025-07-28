#include "cryptsetup.h"

#include "output.h"
#include "subprocess.h"
#include "tpm.h"

#ifndef COMINIT_CRYPTSETUP_DIR
#define COMINIT_CRYPTSETUP_DIR "/usr/sbin/cryptsetup"
#endif

int cominitCryptsetupCreateLuksVolume(char *devCrypt, TPM2B_DIGEST *passphrase) {
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
                          "1",
                          "--key-file",
                          "-",
                          devCrypt,
                          NULL};
    char *env[] = {NULL};

    return cominitSubprocessSpawnAndWrite(argv[0], argv, env, passphrase->buffer, passphrase->size);
}

int cominitCryptsetupOpenLuksVolume(char *devCrypt, TPM2B_DIGEST *passphrase) {
    char *const argv[] = {(char *)COMINIT_CRYPTSETUP_DIR,          "luksOpen", "--key-file", "-", devCrypt,
                          (char *)COMINIT_TPM_SECURE_STORAGE_NAME, NULL};
    char *env[] = {NULL};

    return cominitSubprocessSpawnAndWrite(argv[0], argv, env, passphrase->buffer, passphrase->size);
}