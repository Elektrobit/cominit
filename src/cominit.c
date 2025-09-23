// SPDX-License-Identifier: MIT
/**
 * @file cominit.c
 * @brief Main program implementation of Compact Init (cominit).
 */
#include <errno.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

#ifdef COMINIT_FAKE_HSM
#include "keyring.h"
#endif
#ifdef COMINIT_USE_TPM
#include "tpm.h"
#endif
#include "automount.h"
#include "common.h"
#include "minsetup.h"
#include "output.h"
#include "version.h"

/**
 * Number of times cominit tries to mount the rootfs.
 *
 * If the rootfs is not immediately available, cominit will check again after #COMINIT_ROOT_WAIT_INTERVAL_MILLIS,
 * maximum this many times.
 *
 * TODO: This is a fix for booting on a Raspberry Pi 4. To be more hardware-agnostic we need to make this parameter
 * configurable on the Kernel command line.
 */
#define COMINIT_ROOT_WAIT_TRIES 10uL
/**
 * Interval between tries to mount the rootfs.
 *
 * Unit is milliseconds. See #COMINIT_ROOT_WAIT_TRIES.
 */
#define COMINIT_ROOT_WAIT_INTERVAL_MILLIS 500uL

/**
 * Checks if a string is equal to at least one of two comparison literals.
 *
 * Meant to be used in a loop to check argv for long and short options. Parameters must not be NULL.
 *
 * @param inputParam  pointer to a c string to be compared, may be const
 * @param cmpShort    first comparison string literal, meant for the short form of the option
 * @param cmpLong     second comparison string literal, meant for the long form of the option
 *
 * @return  true if \a inputParam is lexicographically equal to at least one of \a cmpShort and \a cmpLong, false
 *          otherwise
 */
#define cominitParamCheck(inputParam, cmpShort, cmpLong) \
    ((strncmp(inputParam, cmpShort, sizeof(cmpShort)) == 0) || (strncmp(inputParam, cmpLong, sizeof(cmpLong)) == 0))

/**
 * Microsecond sleep function using clock_nanosleep.
 *
 * Uses a monotonic clock with absolute target time to take care of edge cases where sleep is interrupted prematurely
 * by e.g. signals.
 *
 * @param micros  Number of microseconds to wait.
 *
 * @return  0 on success, -1 on error
 */
static inline int cominitMicroSleep(unsigned long long micros);
/**
 * Prints a message indicating cominit's version to stderr.
 */
static void cominitPrintVersion(void);
/**
 * Prints a message about cominit and its intended usage to stderr.
 *
 * Includes version message via cominitPrintVersion().
 */
static void cominitPrintUsage(void);
#ifdef COMINIT_USE_TPM
/**
 * Checks at RT the parsed options to determine if a TPM should be used.
 *
 * @param ctx   Pointer to the structure that receives the parsed options.
 * @return  true if used, false otherwise
 */
static inline bool cominitUseTpm(cominitCliArgs_t *ctx);
#endif
/**
 * Parses a device node from a value in an argument of argv.
 *
 * @param device    Pointer to the structure that the device node is copied to.
 * @param argValue  The parsed value of the argument found in the provided argument vector.
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
static int cominitParseDeviceNode(char *device, const char *argValue);
/**
 * Parses a value from an argument of argv.
 *
 * @param arg    An element of the argument vector.
 * @param param1 The first paramater that should be found in the argument.
 * @param param2 The second paramater that should be found in the argument.
 * @return  Pointer to the value of the argument if one parameter is found, NULL otherwise
 */
static const char *cominitParseArgValue(const char *arg, const char *param1, const char *param2);
/**
 * Tries to discover a valid rootfs either from kernel cmdline or
 * by finding a rootfs GUID in GPT and the corresponding partition.
 *
 * @param argCtx        Pointer to the structure that holds the parsed options.
 * @param rfsMeta       Pointer to the structure that receives the rootfs partition.
 * @param gptDiskRoot   The pointer to a cominitGPTDisk_t struct that receives the disk information.
 * @return  true on success, false otherwise
 */
bool cominitDiscoverRootfs(cominitCliArgs_t *argCtx, cominitRfsMetaData_t *rfsMeta, cominitGPTDisk_t *gptDiskRoot);

/**
 * Compact Init main function.
 *
 * The executable shall be called by the Kernel as init out of initramfs. It will do essential housekeeping tasks to
 * enable mount of the rootfs, including device-mapper setup if necessary. The path to the rootfs is read from the last
 * command line argument. After mounting the rootfs, Compact Init will switch into it and execve into the rootfs init.
 *
 * @return Compact Init shall execve into the rootfs init on success or into a rescue shell
 *         on failure. If a rescue shell cannot be started, Compact Init will terminate and
 *         return an error code.
 */
int main(int argc, char *argv[], char *envp[]) {
    cominitCliArgs_t argCtx = {.visibleLogLevel = COMINIT_LOG_LEVEL_INVALID,
                               .pcrSet = false,
                               .pcrSealCount = 0,
                               .devNodeBlob[0] = '\0',
                               .devNodeCrypt[0] = '\0',
                               .devNodeRootFs[0] = '\0'};
    const char *argValue = NULL;

    for (int i = 0; i < argc; i++) {
        if (cominitParamCheck(argv[i], "-V", "--version")) {
            cominitPrintVersion();
            return EXIT_FAILURE;
        }
        if (cominitParamCheck(argv[i], "-h", "--help")) {
            cominitPrintUsage();
            return EXIT_FAILURE;
        }
        if ((argValue = cominitParseArgValue(argv[i], "root", "cominit.rootfs")) != NULL) {
            if (cominitParseDeviceNode(argCtx.devNodeRootFs, argValue) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires a valid device node ", argv[i]);
                continue;
            }
        }
        if ((argValue = cominitParseArgValue(argv[i], "logLevel", "cominit.logLevel")) != NULL) {
            if (cominitOutputParseLogLevel(&argCtx.visibleLogLevel, argValue) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires a valid log level ", argv[i]);
                continue;
            }
        }
#ifdef COMINIT_USE_TPM
        if ((argValue = cominitParseArgValue(argv[i], "pcrExtend", "cominit.pcrExtend")) != NULL) {
            if (cominitTpmParsePcrIndex(&argCtx, argValue) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires an integer PCR index ", argv[i]);
                continue;
            }
        }
        if ((argValue = cominitParseArgValue(argv[i], "blob", "cominit.blob")) != NULL) {
            if (cominitParseDeviceNode(argCtx.devNodeBlob, argValue) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires a valid device node ", argv[i]);
                continue;
            }
        }
        if ((argValue = cominitParseArgValue(argv[i], "pcrSeal", "cominit.pcrSeal")) != NULL) {
            if (cominitTpmParsePcrIndexes(&argCtx, argValue) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires integer PCR indexes ", argv[i]);
                continue;
            }
        }
        if ((argValue = cominitParseArgValue(argv[i], "crypt", "cominit.crypt")) != NULL) {
            if (cominitParseDeviceNode(argCtx.devNodeCrypt, argValue) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires a valid device node ", argv[i]);
                continue;
            }
        }
#endif
    }
    setsid();
    umask(0);
    cominitOutputSetVisibleLogLevel(argCtx.visibleLogLevel);
    cominitInfoPrint("BaseOS Compact Init version %s started.", cominitGetVersionString());

    /* Mount devtmpfs so we have a minimal system */
    cominitInfoPrint("Setting up minimal environment...");
    if (cominitSetupSysfiles() == -1) {
        cominitErrPrint("Could not setup minimal system/device files. Init failed.");
        goto rescue;
    }

/* In case we are built to emulate a HSM, enroll the standard development key for dm-integrity HMAC in the Kernel
 * user keyring. */
#ifdef COMINIT_FAKE_HSM
    if (cominitKeyringInitFakeHsm() == -1) {
        cominitErrPrint(
            "Could not enroll development key in user keyring. Will continue but dm-integrity with HMAC may fail if "
            "used.");
    }
#endif

    cominitRfsMetaData_t rfsMeta = {0};
    cominitGPTDisk_t gptDiskRoot = {0};

    unsigned long failCount = 0;
    while (cominitDiscoverRootfs(&argCtx, &rfsMeta, &gptDiskRoot) == false) {
        if (failCount < COMINIT_ROOT_WAIT_TRIES) {
            failCount++;
            cominitInfoPrint("No valid rootfs yet found, trying again in %lums.", COMINIT_ROOT_WAIT_INTERVAL_MILLIS);
            cominitMicroSleep((unsigned long long)COMINIT_ROOT_WAIT_INTERVAL_MILLIS * 1000uLL);
        } else {
            cominitErrPrint("No valid rootfs found.");
            goto rescue;
        }
    }

    cominitInfoPrint("Looking for rootfs metadata on partition \'%s\'.", rfsMeta.devicePath);
    if (cominitLoadVerifyMetadata(&rfsMeta, COMINIT_ROOTFS_KEY_LOCATION) == -1) {
        cominitErrPrint("Could not verify partition metadata. Init failed.");
        goto rescue;
    }
    cominitInfoPrint("Rootfs metadata successfully loaded and verified.");

    if (rfsMeta.crypt & COMINIT_CRYPTOPT_CRYPT) {
        cominitErrPrint("Support for dm-crypt not yet available.");
        goto rescue;
    }

#ifdef COMINIT_USE_TPM
    if (argCtx.devNodeCrypt[0] == '\0') {
        cominitInfoPrint("No secureStorage partition given from kernel command line.");
        if (gptDiskRoot.diskName[0] != '\0') {
            if (cominitAutomountFindPartitionOnDisk(&gptDiskRoot, (const char *)COMINIT_SECURE_STORAGE_GUID_TYPE,
                                                    argCtx.devNodeCrypt, sizeof(argCtx.devNodeCrypt)) == EXIT_FAILURE) {
                cominitErrPrint("Could not find secureStorage partition from guid type.");
            }
        } else {
            cominitGPTDisk_t gptDiskNotRoot = {0};
            if (cominitAutomountFindPartition(&gptDiskNotRoot, (const char *)COMINIT_SECURE_STORAGE_GUID_TYPE,
                                              argCtx.devNodeCrypt, sizeof(argCtx.devNodeCrypt)) == EXIT_FAILURE) {
                cominitErrPrint("Could not find secureStorage partition from guid type.");
            }
        }
    }

    if (cominitUseTpm(&argCtx) == true) {
        cominitTpmContext_t tpmCtx;

        cominitInfoPrint("TPM is used");

        int result = cominitInitTpm(&tpmCtx);

        if (result != EXIT_SUCCESS) {
            cominitErrPrint("TPM init failed.");
        } else {
            if (cominitTpmExtendEnabled(&argCtx) == true) {
                result = cominitTpmExtendPCR(&tpmCtx, COMINIT_ROOTFS_KEY_LOCATION, argCtx.pcrIndex);
                if (result != EXIT_SUCCESS) {
                    cominitErrPrint("PCR extention failed.");
                }
            }
            if (cominitTpmSecureStorageEnabled(&argCtx) == true) {
                cominitTpmState_t state = cominitTpmProtectData(&tpmCtx, &argCtx);
                switch (state) {
                    case TpmPolicyFailure:
                        result = cominitTpmHandlePolicyFailure(&tpmCtx);
                        if (result != EXIT_SUCCESS) {
                            cominitErrPrint("Failed to handle policy failure");
                        }
                        break;
                    case Unsealed:
                        break;
                    case Sealed:
                    case TpmFailure:
                    default:
                        cominitErrPrint("TPM failed to set up protected data.");
                        break;
                }
            }
        }
        cominitDeleteTpm(&tpmCtx);
    }
#endif

    /* Set up the rootfs */
    cominitInfoPrint("Setting up rootfs at /newroot...");
    if (cominitSetupRootfs(&rfsMeta) == -1) {
        cominitErrPrint("Could not setup rootfs. Init failed.");
        goto rescue;
    }

#ifdef COMINIT_USE_TPM
    if (cominitTpmSecureStorageEnabled(&argCtx) == true) {
        if (cominitTpmMountSecureStorage() == -1) {
            cominitErrPrint("Mounting of secure storage failed");
        }
    }
#endif

    /* Housekeeping/cleanup before switching to rootfs. For now, just initiate a lazy umount of /dev. */
    cominitInfoPrint("Unmounting system directories...");
    if (cominitCleanupSysfiles() == -1) {
        cominitInfoPrint("Warning: Could not unmount all system/device files.");
    }

    /* Switch into the new rootfs */
    if (cominitSwitchIntoRootfs() == -1) {
        cominitErrPrint("Could not switch into rootfs. Init failed.");
        goto rescue;
    }

    /* if we made it up to here we say goodbye and exec into the rootfs init daemon */
    cominitInfoPrint("Exec into rootfs init...");
    char *const initArgs[] = {"/sbin/init", NULL};
    if (execve("/sbin/init", initArgs, envp) == -1) {
        cominitErrnoPrint("Execve into rootfs init failed.");
    }

rescue:
    /* Start a rescue shell for debugging in case we encountered a fatal error on the way */
    cominitInfoPrint("Exec into rescue shell...");
    char *const shArgs[] = {"/bin/sh", NULL};
    if (execve("/bin/sh", shArgs, envp) == -1) {
        cominitErrnoPrint("Execve into rescue shell failed.");
    }

    /* If execing into rescue shell fails, we will exit in anger and cause a Kernel panic. */
    return EXIT_FAILURE;
}

static inline int cominitMicroSleep(unsigned long long micros) {
    struct timespec t;
    // Use absolute time to avoid issues with interrupted sleeps.
    // Use monotonic clock to avoid issues with clock changes during sleep.
    if (clock_gettime(CLOCK_MONOTONIC, &t) == -1) {
        cominitErrnoPrint("Could not get current time from monotonic clock.");
        return -1;
    }
    // Calculate absolute (monotonic clock) target time when to wake up again.
    t.tv_sec += (time_t)(micros / 1000000uLL);
    unsigned long long nsec = ((micros % 1000000uLL) * 1000uLL) + (unsigned long long)t.tv_nsec;
    t.tv_sec += (time_t)(nsec / 1000000000uLL);
    t.tv_nsec = (long)(nsec % 1000000000uLL);
    int ret = 0;
    do {
        // Good night.
        ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &t, NULL);
    } while (ret == -1 && errno == EINTR);  // Return to sleep if we just got interrupted.
    if (ret == -1) {
        cominitErrnoPrint("Could not sleep for %lluus.", micros);
        return -1;
    }
    return 0;
}

static void cominitPrintVersion(void) {
    printf("Compact Init (cominit) version %s\n", cominitGetVersionString());
}

static void cominitPrintUsage(void) {
    cominitPrintVersion();
    printf(
        "USAGE: cominit must be started by the Kernel as PID 1 from initramfs to set up a rootfs according to its\n"
        "       metadata and then switch into it. The location of the rootfs partition (e.g. "
        "/dev/<blkdevice><partno>)\n"
        "       is determined through an argument passed to cominit by the bootloader on the kernel command line.\n");
}

static int cominitParseDeviceNode(char *device, const char *argValue) {
    int result = EXIT_FAILURE;

    if (device == NULL || argValue == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        if (strncmp(argValue, "/dev/", 5) == 0) {
            size_t devNodeLen = strnlen(argValue, COMINIT_ROOTFS_DEV_PATH_MAX);
            if (devNodeLen < COMINIT_ROOTFS_DEV_PATH_MAX) {
                memcpy(device, argValue, devNodeLen + 1);
                result = EXIT_SUCCESS;
            }
        }
    }

    return result;
}

#ifdef COMINIT_USE_TPM
static inline bool cominitUseTpm(cominitCliArgs_t *argCtx) {
    bool useTpm = false;

    if (cominitTpmExtendEnabled(argCtx) == true) {
        useTpm = true;
    } else if (cominitTpmSecureStorageEnabled(argCtx) == true) {
        useTpm = true;
    }

    return useTpm;
}
#endif

static const char *cominitParseArgValue(const char *arg, const char *key1, const char *key2) {
    const char *value = NULL;

    size_t klen = strlen(key1);
    if (strncmp(arg, key1, klen) == 0 && arg[klen] == '=') {
        value = arg + klen + 1;
    } else {
        klen = strlen(key2);
        if (strncmp(arg, key2, klen) == 0 && arg[klen] == '=') {
            value = arg + klen + 1;
        }
    }

    return value;
}

bool cominitDiscoverRootfs(cominitCliArgs_t *argCtx, cominitRfsMetaData_t *rfsMeta, cominitGPTDisk_t *gptDiskRoot) {
    bool rootFound = false;
    static bool printedOnce = false;

    if (argCtx == NULL || rfsMeta == NULL || gptDiskRoot == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        if (argCtx->devNodeRootFs[0] != '\0') {
            if (!printedOnce) {
                cominitInfoPrint("Rootfs partition %s given from kernel cmdline.", argCtx->devNodeRootFs);
                printedOnce = true;
            }
            struct stat statbuf = {0};
            if (stat(argCtx->devNodeRootFs, &statbuf) == 0) {
                memcpy(rfsMeta->devicePath, argCtx->devNodeRootFs, sizeof(rfsMeta->devicePath));
                rootFound = true;
            }
        } else {
            if (!printedOnce) {
                cominitInfoPrint("No rootfs from kernel cmdline; scanning GPT for rootfs GUID");
                printedOnce = true;
            }
            if (cominitAutomountFindPartition(gptDiskRoot, (const char *)COMINIT_ROOTFS_GUID_TYPE, rfsMeta->devicePath,
                                              sizeof(rfsMeta->devicePath)) == EXIT_SUCCESS) {
                rootFound = true;
            }
        }
    }

    return rootFound;
}
