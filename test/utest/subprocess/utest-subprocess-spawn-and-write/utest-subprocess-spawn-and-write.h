// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn-and-write.h
 * @brief Header declaring cmocka unit test functions for cominitSubprocessSpawnAndWrite().
 */
#ifndef __UTEST_SUBPROCESS_SPAWN_H__
#define __UTEST_SUBPROCESS_SPAWN_H__

#include "common.h"
#include "subprocess.h"

/**
 * Unit test for cominitSubprocessSpawnAndWrite() successful code path.
 * @param state
 */
void cominitSubprocessSpawnAndWriteTestSuccess(void **state);

/**
 * Unit test for cominitSubprocessSpawnAndWrite() if parameters are not initialized.
 * @param state
 */
void cominitSubprocessSpawnAndWriteTestParamFailure(void **state);

#endif /* __UTEST_SUBPROCESS_SPAWN_H__ */
