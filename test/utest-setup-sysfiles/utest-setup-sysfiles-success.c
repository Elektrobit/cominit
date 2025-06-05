// SPDX-License-Identifier: MIT
/**
 * @file utest-setup-sysfiles-success.c
 * @brief Implementation of an success case unit test for cominitSetupSysfiles().
 */
#include <errno.h>

#include "common.h"
#include "minsetup.h"
#include "mock_mount.h"
#include "unit_test.h"
#include "utest-setup-sysfiles.h"

void cominitSetupSysfilesTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    // Check success case where we create and mount /dev.
    expect_string(__wrap_mkdir, pathName, MNT_TGT_DEV);
    expect_value(__wrap_mkdir, mode, DIR_MODE_DEV);
    will_return(__wrap_mkdir, 0);
    will_return(__wrap_mkdir, 0);
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_DEV);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_DEV);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_DEV);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, 0);
    will_return(__wrap_mount, 0);

    // Check success case where we create and mount /proc.
    expect_string(__wrap_mkdir, pathName, MNT_TGT_PROC);
    expect_value(__wrap_mkdir, mode, DIR_MODE_PROC);
    will_return(__wrap_mkdir, 0);
    will_return(__wrap_mkdir, 0);
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_PROC);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_PROC);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_PROC);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, 0);
    will_return(__wrap_mount, 0);

    assert_int_equal(cominitSetupSysfiles(), 0);
}

void cominitSetupSysfilesTestSuccessAlreadyMounted(void **state) {
    COMINIT_PARAM_UNUSED(state);

    // Check success where /dev is already mounted.
    expect_string(__wrap_mkdir, pathName, MNT_TGT_DEV);
    expect_value(__wrap_mkdir, mode, DIR_MODE_DEV);
    will_return(__wrap_mkdir, EEXIST);
    will_return(__wrap_mkdir, -1);  // We pretend mkdir() just runs fine here as we check the EEXIST case already above.
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_DEV);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_DEV);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_DEV);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, EBUSY);
    will_return(__wrap_mount, -1);

    // Check success where /proc is already mounted.
    expect_string(__wrap_mkdir, pathName, MNT_TGT_PROC);
    expect_value(__wrap_mkdir, mode, DIR_MODE_PROC);
    will_return(__wrap_mkdir, EEXIST);
    will_return(__wrap_mkdir, -1);  // We pretend mkdir() just runs fine here as we check the EEXIST case already above.
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_PROC);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_PROC);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_PROC);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, EBUSY);
    will_return(__wrap_mount, -1);
    assert_int_equal(cominitSetupSysfiles(), 0);
}

void cominitSetupSysfilesTestSuccessDirExists(void **state) {
    COMINIT_PARAM_UNUSED(state);

    // Check success where /dev already exists as a directory but is not yet mounted
    expect_string(__wrap_mkdir, pathName, MNT_TGT_DEV);
    expect_value(__wrap_mkdir, mode, DIR_MODE_DEV);
    will_return(__wrap_mkdir, EEXIST);
    will_return(__wrap_mkdir, -1);
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_DEV);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_DEV);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_DEV);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, 0);
    will_return(__wrap_mount, 0);

    // Check success where /dev already exists as a directory but is not yet mounted
    expect_string(__wrap_mkdir, pathName, MNT_TGT_PROC);
    expect_value(__wrap_mkdir, mode, DIR_MODE_PROC);
    will_return(__wrap_mkdir, EEXIST);
    will_return(__wrap_mkdir, -1);
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_PROC);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_PROC);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_PROC);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, 0);
    will_return(__wrap_mount, 0);
    assert_int_equal(cominitSetupSysfiles(), 0);
}
