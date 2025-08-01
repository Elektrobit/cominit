// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn-param-failure.c
 * @brief Implementation of a failure case unit test for cominitSubprocessSpawn().
 */

#include <cmocka_extensions/cmocka_extensions.h>

#include "utest-subprocess-spawn.h"

void cominitSubprocessSpawnTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    char *const argv[] = {"/bin/true", NULL};
    char *const env[] = {NULL};

    assert_int_not_equal(cominitSubprocessSpawn(NULL, argv, env), 0);

    assert_int_not_equal(cominitSubprocessSpawn(argv[0], NULL, env), 0);

    assert_int_not_equal(cominitSubprocessSpawn(argv[0], argv, NULL), 0);
}
