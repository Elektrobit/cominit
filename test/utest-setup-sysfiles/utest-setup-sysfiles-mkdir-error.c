// SPDX-License-Identifier: MIT
/**
 * @file utest-setup-sysfiles-mkdir-error.c
 * @brief Implementation of an error case unit test for cominitSetupSysfiles().
 */
#include <errno.h>

#include "common.h"
#include "minsetup.h"
#include "mock_mount.h"
#include "unit_test.h"
#include "utest-setup-sysfiles.h"

void cominitSetupSysfilesTestMkdirError(void **state) {
    COMINIT_PARAM_UNUSED(state);
    expect_string(__wrap_mkdir, pathName, MNT_TGT);
    expect_value(__wrap_mkdir, mode, DIR_MODE);
    will_return(__wrap_mkdir, EINVAL);  // Use something that is not EEXIST, i.e. not already created.
    will_return(__wrap_mkdir, -1);
    assert_int_equal(cominitSetupSysfiles(), -1);
}
