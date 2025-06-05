// SPDX-License-Identifier: MIT
/**
 * @file utest-cleanup-sysfiles.h
 * @brief Header declaring cmocka unit test functions for cominitSetupSysfiles().
 */
#ifndef __UTEST_SETUP_SYSFILES_H__
#define __UTEST_SETUP_SYSFILES_H__

#include <sys/mount.h>

#define MNT_SRC "none"                     ///< Mount source parameter
#define MNT_TGT_DEV "/dev"                 ///< Mount target parameter
#define MNT_TGT_PROC "/proc"               ///< Mount target parameter
#define MNT_TYPE_DEV "devtmpfs"            ///< Mount file system type
#define MNT_TYPE_PROC "proc"                ///< Mount file system type
#define MNT_FLAGS_DEV (MS_NOEXEC | MS_NOSUID)  ///< Mount flags for the devtmpfs
#define MNT_FLAGS_PROC (MS_NODEV | MS_NOEXEC | MS_NOSUID)  ///< Mount flags for the proc
#define MNT_DATA NULL                      ///< Mount data parameter
#define DIR_MODE_DEV 0755                  ///< Directory mode for the devtmpfs
#define DIR_MODE_PROC 0555                 ///< Directory mode for the proc


/**
 * Unit test for cominitSetupSysfiles() mount() error code path.
 *
 * Needs __wrap_mount() mock function. The mock function is configured to behave as an unsuccessful call to mount(),
 * excluding EBUSY.
 */
void cominitSetupSysfilesTestMountError(void **state);
/**
 * Unit test for cominitSetupSysfiles() mkdir() error code path.
 *
 * Needs __wrap_mkdir() mock function. The mock function is configured to behave as an unsuccessful call to mkdir(),
 * excluding EEXIST.
 */
void cominitSetupSysfilesTestMkdirError(void **state);
/**
 * Unit test for cominitSetupSysfiles() successful code path.
 *
 * Needs __wrap_mount() and __wrap_mkdir() mock functions. The mock functions are configured to behave successfully.
 */
void cominitSetupSysfilesTestSuccess(void **state);
/**
 * Unit test for cominitSetupSysfiles() successful code path where /dev already exists as a directory.
 *
 * Needs __wrap_mount() and __wrap_mkdir mock functions. The mkdir function is configured to behave as if the directory
 * already exists, mount is successful.
 */
void cominitSetupSysfilesTestSuccessDirExists(void **state);
/**
 * Unit test for cominitSetupSysfiles() successful code path where /dev is already mounted.
 *
 * Needs __wrap_mount() and __wrap_mkdir mock functions. The mount function will behave as though /dev is already
 * mounted. mkdir will report success, so we do not have to set its errno (that code path is tested by
 * cominitSetupSysfilesTestSuccessDirExists()).
 */
void cominitSetupSysfilesTestSuccessAlreadyMounted(void **state);

#endif /* __UTEST_SETUP_SYSFILES_H__ */
