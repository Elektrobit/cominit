// SPDX-License-Identifier: MIT
/**
 * @file minsetup.c
 * @brief Implementation of minimal system setup.
 */
#include "minsetup.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mount.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "common.h"
#include "dmctl.h"
#include "output.h"

#define __USE_XOPEN_EXTENDED 1  // needed so glibc has nftw(), actually not needed for musl
#include <ftw.h>

/**
 * Macro for error state in minsetup.c functions.
 *
 * If expression \a expr evaluates to 0, the calling function will exit with return code -1 and print an error message
 * containing the offending expression and the state of errno using the cominitErrnoPrint() macro. Meant to be used with
 * (system) functions that set errno on error.
 */
#define cominitFailIf(expr)                   \
    errno = 0;                                \
    do {                                      \
        if (expr) {                           \
            cominitErrnoPrint("`" #expr "'"); \
            return -1;                        \
        }                                     \
    } while (0)

/**
 * Function to recursively remove files and directories through nftw().
 *
 * Will print warnings if any files/dirs are not removable but will never outright fail.
 *
 * For an explanation of the parameters, see the nftw() manpage.
 *
 * @return  Always 0.
 */
static int cominitNftwRemove(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf);

/* Setup of minimal environment we need in initramfs. For now, devtmpfs and proc are enough for us. */
int cominitSetupSysfiles(void) {
    umask(0);

    if (mkdir("/dev", 0755) == -1) {
        if (errno != EEXIST) {
            cominitErrnoPrint("Could not create /dev directory.");
            return -1;
        }
    }
    if (mount("none", "/dev", "devtmpfs", MS_NOEXEC | MS_NOSUID, NULL) == -1) {
        if (errno != EBUSY) {
            cominitErrnoPrint("Could not mount devtmpfs.");
            return -1;
        }
        cominitInfoPrint("/dev is already mounted. Skipping.");
    }

    if (mkdir("/proc", 0555) == -1) {
        if (errno != EEXIST) {
            cominitErrnoPrint("Could not create /proc directory: ");
            return -1;
        }
    }
    if (mount("none", "/proc", "proc", MS_NODEV | MS_NOEXEC | MS_NOSUID, NULL) == -1) {
        if (errno != EBUSY) {
            cominitErrnoPrint("Could not mount procfs.");
            return -1;
        }
        cominitInfoPrint("/proc is already mounted. Skipping.");
    }

    umask(0022);
    return 0;
}

/* Perform 'lazy' unmount of devtmpfs as it may be busy, this will result in a proper unmount during execve to rootfs
 * init at the latest. (Even if that wasn't the case remounting at /newroot/dev is not a problem) */
int cominitCleanupSysfiles(void) {
    cominitFailIf(umount2("/dev", MNT_DETACH) == -1);
    return 0;
}

int cominitSetupRootfs(cominitRfsMetaData_t *rfsMeta) {
    if (rfsMeta == NULL) {
        cominitErrPrint("Input parameters must not be NULL.");
        return -1;
    }

    if (!rfsMeta->ro && !strcmp(rfsMeta->fsType, "squashfs")) {
        cominitErrPrint("A squashfs rootfs can only be mounted read-only.");
        return -1;
    }

    if (rfsMeta->crypt != COMINIT_CRYPTOPT_NONE && cominitSetupDmDevice(rfsMeta) == -1) {
        cominitErrPrint("Could not set up rootfs using the device mapper.");
        return -1;
    }

    if (mkdir("/newroot", 0755) == -1) {
        if (errno != EEXIST) {
            cominitErrnoPrint("Could not create /newroot directory: ");
            return -1;
        }
    }

    cominitFailIf(mount(rfsMeta->devicePath, "/newroot", rfsMeta->fsType, (rfsMeta->ro) ? MS_RDONLY : 0, NULL) == -1);
    return 0;
}

static int cominitNftwRemove(const char *fpath, const struct stat *sb, int tflag, struct FTW *ftwbuf) {
    COMINIT_PARAM_UNUSED(ftwbuf);
    COMINIT_PARAM_UNUSED(sb);
    switch (tflag) {
        case FTW_F:
        case FTW_SL:
            if (unlink(fpath) == -1) {
                cominitErrnoPrint("Could not remove file or symlink \'%s\'.", fpath);
            }
            break;
        case FTW_DP:
            if (ftwbuf->level > 0) {
                if (rmdir(fpath) == -1) {
                    cominitErrnoPrint("Could not remove directory \'%s\'.", fpath);
                }
            }

            break;
        case FTW_D:
            break;
        case FTW_DNR:
        case FTW_NS:
            cominitInfoPrint("Warning: Could not access path \'%s\'.", fpath);
            break;
        case FTW_SLN:
        default:
            cominitInfoPrint("Warning: Got unexpected type flag from nftw (%d) for path \'%s\'.", tflag, fpath);
            break;
    }
    return 0;
}

int cominitSwitchIntoRootfs(void) {
    cominitInfoPrint("Freeing up initramfs...");
    if (nftw("/", cominitNftwRemove, 32, FTW_DEPTH | FTW_PHYS | FTW_MOUNT) == -1) {
        cominitInfoPrint("Warning: Some parts of initramfs could not be deleted.");
    }
    cominitInfoPrint("Switching root to /newroot...");
    if (chdir("/newroot") == -1) {
        cominitErrnoPrint("Could not cd to /newroot.");
        return -1;
    }
    if (mount("/newroot", "/", NULL, MS_MOVE, NULL) == -1) {
        cominitErrnoPrint("Could not move rootfs mount to /.");
        return -1;
    }
    if (chroot(".") == -1) {
        cominitErrnoPrint("Could not chroot into /newroot.");
        return -1;
    }
    if (chdir("/") == -1) {
        cominitErrnoPrint("Could not cd after chroot.");
        return -1;
    }
    return 0;
}
