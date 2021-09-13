// SPDX-License-Identifier: MIT
/**
 * @file utest-cleanup-sysfiles-success.c
 * @brief Implementation of an success case unit test for cominitCleanupSysfiles().
 */
#include <sys/mount.h>

#include "common.h"
#include "minsetup.h"
#include "mock_umount2.h"
#include "unit_test.h"
#include "utest-cleanup-sysfiles.h"

void cominitCleanupSysfilesTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    expect_string(__wrap_umount2, target, "/dev");
    expect_value(__wrap_umount2, flags, MNT_DETACH);
    will_return(__wrap_umount2, 0);
    assert_int_equal(cominitCleanupSysfiles(), 0);
}
