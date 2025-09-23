// SPDX-License-Identifier: MIT
/**
 * @file output.c
 * @brief Debug/Log output implementation.
 */
#include "output.h"

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOG_LEVEL COMINIT_LOG_LEVEL_INFO

/**
 * Structure that holds a log level entry.
 */
typedef struct cominitLogLevelEntry {
    const char *name;    ///< The name of the log level.
    const char *prefix;  ///< The prefix printed in a message of this log level.
} cominitLogLevelEntry_t;

/**
 * Structure that holds the available log levels and the current visible log level.
 */
typedef struct cominitLogContext {
    cominitLogLevelEntry_t logLevelEntry[COMINIT_LOG_LEVEL_COUNT];  ///< The available log levels.
    cominitLogLevelE_t visibleLevel;                                ///< The current visible log level
} cominitLogContext_t;

static cominitLogContext_t cominitLogContext = {
    .logLevelEntry =
        {
            [COMINIT_LOG_LEVEL_NONE] = {.name = "NONE", .prefix = NULL},
            [COMINIT_LOG_LEVEL_ERR] = {.name = "ERROR", .prefix = "ERROR: "},
            [COMINIT_LOG_LEVEL_WARN] = {.name = "WARNING", .prefix = "WARNING: "},
            [COMINIT_LOG_LEVEL_INFO] = {.name = "INFO", .prefix = NULL},
            [COMINIT_LOG_LEVEL_DEBUG] = {.name = "DEBUG", .prefix = "DEBUG: "},
            [COMINIT_LOG_LEVEL_SENSITIVE] = {.name = "SENSITIVE", .prefix = "SENSITIVE: "},
        },
    .visibleLevel = DEFAULT_LOG_LEVEL};

void cominitOutputSetVisibleLogLevel(cominitLogLevelE_t cominitLogLevel) {
    if (cominitLogLevel == COMINIT_LOG_LEVEL_INVALID) {
        cominitLogContext.visibleLevel = DEFAULT_LOG_LEVEL;
    } else {
        cominitLogContext.visibleLevel = cominitLogLevel;
    }
}

int cominitOutputParseLogLevel(cominitLogLevelE_t *logLevel, const char *argValue) {
    int result = EXIT_FAILURE;

    if (argValue == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        *logLevel = COMINIT_LOG_LEVEL_INVALID;
        char *end = NULL;
        errno = 0;
        unsigned int visibleLogLevel = strtoul(argValue, &end, 0);
        if (!errno && *end == '\0' && visibleLogLevel < COMINIT_LOG_LEVEL_COUNT) {
            result = EXIT_SUCCESS;
            *logLevel = (cominitLogLevelE_t)visibleLogLevel;
        }
    }

    return result;
}

int cominitOutputLogFunc(cominitLogLevelE_t logLevel, const char *file, const char *func, int line, bool printErrno,
                         const char *format, ...) {
    int ret = 0;

    if (logLevel <= COMINIT_LOG_LEVEL_NONE || logLevel >= COMINIT_LOG_LEVEL_COUNT) {
        return ret;
    }

    if (logLevel > cominitLogContext.visibleLevel) {
        return ret;
    }

#if !defined(COMINIT_ENABLE_SENSITIVE_LOGGING)
    if (logLevel >= COMINIT_LOG_LEVEL_SENSITIVE) {
        return ret;
    }
#endif

    int cCount = 0;
    va_list args;

    if (logLevel == COMINIT_LOG_LEVEL_INFO) {
        ret = fprintf(stderr, COMINIT_PRINT_PREFIX);

    } else {
        ret = fprintf(stderr, COMINIT_PRINT_PREFIX "(%s:%s:%d) %s", file, func, line,
                      cominitLogContext.logLevelEntry[logLevel].prefix);
    }
    if (ret < 0) {
        return ret;
    }
    cCount += ret;

    va_start(args, format);
    ret = vfprintf(stderr, format, args);
    va_end(args);
    if (ret < 0) {
        return ret;
    }
    cCount += ret;

    ret = fprintf(stderr, "\n");
    if (ret < 0) {
        return ret;
    }
    cCount += ret;

    if (printErrno == true) {
        ret = fprintf(stderr, " Errno: %s\n", strerror(errno));
        if (ret < 0) {
            return ret;
        }
        cCount += ret;
    }

    return cCount;
}
