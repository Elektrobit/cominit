// SPDX-License-Identifier: MIT
/**
 * @file utest-setup-sysfiles-mount-error.c
 * @brief Implementation of an error case unit test for cominitSetupSysfiles().
 */
#include <errno.h>

#include "common.h"
#include "minsetup.h"
#include "mock_mount.h"
#include "unit_test.h"
#include "utest-setup-sysfiles.h"

void cominitSetupSysfilesTestMountError(void **state) {
    COMINIT_PARAM_UNUSED(state);
    expect_string(__wrap_mkdir, pathName, MNT_TGT_DEV);
    expect_value(__wrap_mkdir, mode, DIR_MODE_DEV);
    will_return(__wrap_mkdir, 0);
    will_return(__wrap_mkdir, 0);
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_DEV);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_DEV);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_DEV);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, EINVAL);  // Use something that is not EBUSY, i.e. not already mounted.
    will_return(__wrap_mount, -1);
    assert_int_equal(cominitSetupSysfiles(), -1);

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
    expect_string(__wrap_mkdir, pathName, MNT_TGT_PROC);
    expect_value(__wrap_mkdir, mode, DIR_MODE_PROC);
    will_return(__wrap_mkdir, 0);
    will_return(__wrap_mkdir, 0);
    expect_string(__wrap_mount, source, MNT_SRC);
    expect_string(__wrap_mount, target, MNT_TGT_PROC);
    expect_string(__wrap_mount, fileSystemType, MNT_TYPE_PROC);
    expect_value(__wrap_mount, mountFlags, MNT_FLAGS_PROC);
    expect_value(__wrap_mount, data, MNT_DATA);
    will_return(__wrap_mount, EINVAL);  // Use something that is not EBUSY, i.e. not already mounted.
    will_return(__wrap_mount, -1);
    assert_int_equal(cominitSetupSysfiles(), -1);
}
