// SPDX-License-Identifier: MIT
/**
 * @file utest-output-parse-log-level.h
 * @brief Header declaring cmocka unit test functions for cominitOutputParseLogLevel().
 */
#ifndef __UTEST_OUTPUT_PARSE_LOG_LEVEL_H__
#define __UTEST_OUTPUT_PARSE_LOG_LEVEL_H__

#include <sys/mount.h>

/**
 * Unit test for cominitOutputParseLogLevel() successful code path.
 * @param state
 */
void cominitOutputParseLogLevelTestSuccess(void **state);

/**
 * Unit test that simulates different non integer parameters
 * @param state
 */
void cominitOutputParseLogLevelTestFailure(void **state);

#endif /* __UTEST_OUTPUT_PARSE_LOG_LEVEL_H__ */
