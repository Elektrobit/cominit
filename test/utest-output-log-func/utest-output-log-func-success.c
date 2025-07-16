// SPDX-License-Identifier: MIT
/**
 * @file utest-output-log-func-success.c
 * @brief Implementation of a success case unit test for cominitOutputLogFunc().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "utest-output-log-func.h"

static const char *cominitLogFile = "file";
static const char *cominitLogFunc = "func";
static int cominitLogLine = 42;
static const char *cominitLogPayload = "test 123";

int cominitTriggerLogOutputTest(logCtx *ctx, cominitLogLevelE_t logLevel, cominitLogLevelE_t visibleLogLevel) {
    int result = -1;

    if (pipe(ctx->pipefd) == 0) {
        pid_t pid = fork();
        if (pid < 0) {
            close(ctx->pipefd[0]);
            close(ctx->pipefd[1]);
        }
        if (pid == 0) {
            close(ctx->pipefd[0]);
            dup2(ctx->pipefd[1], STDERR_FILENO);
            close(ctx->pipefd[1]);
            cominitOutputSetVisibleLogLevel(visibleLogLevel);
            cominitOutputLogFunc(logLevel, cominitLogFile, cominitLogFunc, cominitLogLine, false, "%s",
                                 cominitLogPayload);
            fflush(stderr);
            _exit(EXIT_SUCCESS);
        }
        if (pid > 0) {
            close(ctx->pipefd[1]);
            ctx->child = pid;
            int status = -1;
            waitpid(ctx->child, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                result = 0;
            }
        }
    }

    return result;
}

void cominitOutputLogFuncTestSuccessPrint(void **state) {
    COMINIT_PARAM_UNUSED(state);

    logCtx ctx = {0};
    const char *logPrefix[COMINIT_LOG_LEVEL_COUNT] = {0};

#ifdef COMINIT_ENABLE_SENSITIVE_LOGGING
    logPrefix[COMINIT_LOG_LEVEL_SENSITIVE] = "SENSITIVE: ";
#else
    logPrefix[COMINIT_LOG_LEVEL_ERR] = "ERROR: ";
    logPrefix[COMINIT_LOG_LEVEL_WARN] = "WARNING: ";
    logPrefix[COMINIT_LOG_LEVEL_INFO] = NULL;
    logPrefix[COMINIT_LOG_LEVEL_DEBUG] = "DEBUG: ";
#endif

    for (size_t i = 0; i < COMINIT_LOG_LEVEL_COUNT; ++i) {
        if (logPrefix[i] != NULL) {
            memset(&ctx, 0, sizeof(ctx));
            /* should be printed because log level == visible log level */
            if (cominitTriggerLogOutputTest(&ctx, i, i) == 0) {
                if (i == COMINIT_LOG_LEVEL_INFO) {
                    char fmt[] = "%s %s%s\n";
                    snprintf(ctx.expectedPrintOut, sizeof(ctx.expectedPrintOut), fmt, "[COMINIT]", logPrefix[i],
                             cominitLogPayload);
                } else {
                    char fmt[] = "%s (%s:%s:%d) %s%s\n";
                    snprintf(ctx.expectedPrintOut, sizeof(ctx.expectedPrintOut), fmt, "[COMINIT]", cominitLogFile,
                             cominitLogFunc, cominitLogLine, logPrefix[i], cominitLogPayload);
                }

                ssize_t n = read(ctx.pipefd[0], ctx.readBuffer, sizeof(ctx.readBuffer) - 1);
                assert_int_equal(n, strlen(ctx.expectedPrintOut));

                assert_string_equal(ctx.readBuffer, ctx.expectedPrintOut);
            }

            if (ctx.pipefd[0] > 0) {
                close(ctx.pipefd[0]);
            }
        }
    }
}

void cominitOutputLogFuncTestSuccessNoPrint(void **state) {
    COMINIT_PARAM_UNUSED(state);

    logCtx ctx = {0};
    int result = -1;
    const char *logPrefix[COMINIT_LOG_LEVEL_COUNT] = {0};

#ifndef COMINIT_ENABLE_SENSITIVE_LOGGING
    logPrefix[COMINIT_LOG_LEVEL_SENSITIVE] = "SENSITIVE: ";
#else
    logPrefix[COMINIT_LOG_LEVEL_NONE] = "";
    logPrefix[COMINIT_LOG_LEVEL_ERR] = "ERROR: ";
    logPrefix[COMINIT_LOG_LEVEL_WARN] = "WARNING: ";
    logPrefix[COMINIT_LOG_LEVEL_INFO] = NULL;
    logPrefix[COMINIT_LOG_LEVEL_DEBUG] = "DEBUG: ";
#endif

    for (size_t i = 0; i < COMINIT_LOG_LEVEL_COUNT; ++i) {
        if (logPrefix[i] != NULL) {
            memset(&ctx, 0, sizeof(ctx));
            if (i == COMINIT_LOG_LEVEL_SENSITIVE || i == COMINIT_LOG_LEVEL_NONE) {
                /* special cases: should not be printed even if log level < visible log level */
                result = cominitTriggerLogOutputTest(&ctx, i, i + 1);
            } else {
                /* should not be printed because log level > visible log level */
                result = cominitTriggerLogOutputTest(&ctx, i + 1, i);
            }
            if (result == 0) {
                ssize_t n = read(ctx.pipefd[0], ctx.readBuffer, sizeof(ctx.readBuffer) - 1);
                assert_int_equal(n, 0);

                assert_string_equal(ctx.readBuffer, "");
            }

            if (ctx.pipefd[0] > 0) {
                close(ctx.pipefd[0]);
            }
        }
    }
}
