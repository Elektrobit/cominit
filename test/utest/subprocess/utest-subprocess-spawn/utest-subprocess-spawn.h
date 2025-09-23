// SPDX-License-Identifier: MIT
/**
 * @file utest-subprocess-spawn.h
 * @brief Header declaring cmocka unit test functions for cominitSubprocessSpawn().
 */
#ifndef __UTEST_SUBPROCESS_SPAWN_H__
#define __UTEST_SUBPROCESS_SPAWN_H__

#include "common.h"
#include "subprocess.h"

/**
 * Unit test for cominitSubprocessSpawn() successful code path.
 * @param state
 */
void cominitSubprocessSpawnTestSuccess(void **state);

/**
 * Unit test for cominitSubprocessSpawn() if parameters are not initialized.
 * @param state
 */
void cominitSubprocessSpawnTestParamFailure(void **state);

#endif /* __UTEST_SUBPROCESS_SPAWN_H__ */
