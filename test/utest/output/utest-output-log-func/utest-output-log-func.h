// SPDX-License-Identifier: MIT
/**
 * @file utest-output-log-func.h
 * @brief Header declaring cmocka unit test functions for cominitOutputLogFunc().
 */
#ifndef __UTEST_OUTPUT_LOG_FUNC_H__
#define __UTEST_OUTPUT_LOG_FUNC_H__

#include <sys/wait.h>

#include "output.h"

typedef struct {
    int pipefd[2];
    pid_t child;
    char readBuffer[256];
    char expectedPrintOut[128];
} logCtx;

/**
 * Unit test for cominitOutputLogFunc() successful code path.
 * @param state
 */
void cominitOutputLogFuncTestSuccessPrint(void **state);
void cominitOutputLogFuncTestSuccessNoPrint(void **state);

#endif /* __UTEST_OUTPUT_LOG_FUNC_H__ */
