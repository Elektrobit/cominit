// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn-and-write-param-failure.c
 * @brief Implementation of a failure case unit test for cominitSubprocessSpawnAndWrite().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <string.h>

#include "utest-subprocess-spawn-and-write.h"

void cominitSubprocessSpawnAndWriteTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    char *const argv[] = {"/bin/cat", NULL};
    char *const env[] = {NULL};
    const char data[] = {"test secret"};
    size_t dataSize = strlen(data);

    assert_int_not_equal(cominitSubprocessSpawnAndWrite(NULL, argv, env, data, dataSize), 0);

    assert_int_not_equal(cominitSubprocessSpawnAndWrite(argv[0], NULL, env, data, dataSize), 0);

    assert_int_not_equal(cominitSubprocessSpawnAndWrite(argv[0], argv, NULL, data, dataSize), 0);

    assert_int_not_equal(cominitSubprocessSpawnAndWrite(argv[0], argv, env, NULL, dataSize), 0);

    assert_int_not_equal(cominitSubprocessSpawnAndWrite(argv[0], argv, env, data, 0), 0);
}
