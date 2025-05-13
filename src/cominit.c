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
/**
 * Checks at RT the parsed options to determine if a TPM should be used.
 *
 * @param ctx   Pointer to the structure that receives the parsed options.
 * @return  0 on success, 1 otherwise
 */
static inline int cominitUseTpm(cominitCliArgs_t *ctx);

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
#ifdef COMINIT_USE_TPM
    cominitCliArgs_t argCtx = {0};
#endif

    for (int i = 0; i < argc; i++) {
        if (cominitParamCheck(argv[i], "-V", "--version")) {
            cominitPrintVersion();
            return EXIT_FAILURE;
        }
        if (cominitParamCheck(argv[i], "-h", "--help")) {
            cominitPrintUsage();
            return EXIT_FAILURE;
        }
#ifdef COMINIT_USE_TPM
        if (cominitParamCheck(argv[i], "pcrExtend", "cominit.pcrExtend")) {
            if (cominitTpmParsePcrIndex(&argCtx, argc, argv, i) == EXIT_FAILURE) {
                cominitErrPrint("\'%s\' requires an integer PCR index ", argv[i]);
                continue;
            }
        }
#endif
    }
    setsid();
    umask(0);
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
    if (cominitInitFakeHsm() == -1) {
        cominitErrPrint(
            "Could not enroll development key in user keyring. Will continue but dm-integrity with HMAC may fail if "
            "used.");
    }
#endif

    cominitRfsMetaData_t rfsMeta;

    /* Check if we got an absolute path to an existing device node as the last argument on the Kernel command line */
    char *lastArg = argv[argc - 1];
    if (lastArg != NULL && strncmp(lastArg, "/dev", 4) == 0) {
        struct stat statbuf;
        unsigned long failCount = 0;
        while (stat(lastArg, &statbuf) == -1) {
            if ((errno == ENOENT || errno == EACCES) && failCount < COMINIT_ROOT_WAIT_TRIES) {
                cominitInfoPrint("Rootfs at '%s' not yet available or accessible, trying again in %lums.", lastArg,
                                 COMINIT_ROOT_WAIT_INTERVAL_MILLIS);
                failCount++;
                cominitMicroSleep((unsigned long long)COMINIT_ROOT_WAIT_INTERVAL_MILLIS * 1000uLL);
            } else {
                cominitErrnoPrint("Could not stat given rootfs location (\'%s\').", lastArg);
                goto rescue;
            }
        }
    } else {
        cominitErrPrint("No rootfs partition provided by the bootloader.");
        goto rescue;
    }

    char *endPtr = stpncpy(rfsMeta.devicePath, lastArg, sizeof(rfsMeta.devicePath) - 1);
    if (*endPtr != '\0') {
        cominitErrPrint("Given rootfs location from command line (\'%s\') is too long.", lastArg);
        goto rescue;
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
    if (cominitUseTpm(&argCtx) == EXIT_SUCCESS) {
        cominitTpmContext_t tpmCtx;

        cominitInfoPrint("TPM is used");

        int result = cominitInitTpm(&tpmCtx);

        if (result != EXIT_SUCCESS) {
            cominitErrPrint("TPM init failed.");
        } else {
            if (argCtx.pcrSet == true) {
                result = cominitTpmExtendPCR(&tpmCtx, COMINIT_ROOTFS_KEY_LOCATION, argCtx.pcrIndex);
                if (result != EXIT_SUCCESS) {
                    cominitErrPrint("PCR extention failed.");
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

static inline int cominitUseTpm(cominitCliArgs_t *ctx) {
    int result = EXIT_FAILURE;

    if (ctx == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        if (ctx->pcrSet == true) {
            result = EXIT_SUCCESS;
        }
    }

    return result;
}

