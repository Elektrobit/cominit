// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn-and-write-success.c
 * @brief Implementation of a success case unit test for cominitSubprocessSpawnAndWrite().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <string.h>

#include "utest-subprocess-spawn-and-write.h"

void cominitSubprocessSpawnAndWriteTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    char *const argv[] = {"/bin/cat", NULL};
    char *const env[] = {NULL};
    const char data[] = {"test secret"};
    size_t dataSize = strlen(data);

    assert_int_equal(cominitSubprocessSpawnAndWrite(argv[0], argv, env, data, dataSize), 0);
}
