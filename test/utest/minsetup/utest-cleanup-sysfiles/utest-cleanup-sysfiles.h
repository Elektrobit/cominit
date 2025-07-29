// SPDX-License-Identifier: MIT
/**
 * @file utest-cleanup-sysfiles.h
 * @brief Header declaring cmocka unit test functions for cominitCleanupSysfiles().
 */
#ifndef __UTEST_CLEANUP_SYSFILES_H__
#define __UTEST_CLEANUP_SYSFILES_H__

/**
 * Unit test for cominitCleanupSysfiles() umount2() error code path.
 *
 * Needs __wrap_umount2() mock function. The mock function is configured to behave as an unsuccessful call to umount2().
 */
void cominitCleanupSysfilesTestUmountError(void **state);
/**
 * Unit test for cominitCleanupSysfiles() successful code path.
 *
 * Needs __wrap_umount2() mock function. The mock function is configured to behave as a successful call to umount2().
 */
void cominitCleanupSysfilesTestSuccess(void **state);

#endif /* __UTEST_CLEANUP_SYSFILES_H__ */
