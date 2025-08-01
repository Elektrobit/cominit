// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn-success.c
 * @brief Implementation of a success case unit test for cominitSubprocessSpawn().
 */

#include <cmocka_extensions/cmocka_extensions.h>

#include "utest-subprocess-spawn.h"

void cominitSubprocessSpawnTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    char *const argv[] = {"/bin/true", NULL};
    char *const env[] = {NULL};

    assert_int_equal(cominitSubprocessSpawn(argv[0], argv, env), 0);
}
