// SPDX-License-Identifier: MIT
/**
 * @file output.c
 * @brief Debug/Log output implementation.
 */
#include "output.h"

#include <errno.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

int cominitInfoPrint(const char *format, ...) {
    int ret = 0;
    int cCount = 0;
    va_list args;
    ret = printf(COMINIT_PRINT_PREFIX);
    if (ret < 0) {
        return ret;
    }
    cCount += ret;
    va_start(args, format);
    ret = vprintf(format, args);
    if (ret < 0) {
        va_end(args);
        return ret;
    }
    va_end(args);
    cCount += ret;
    ret = printf("\n");
    if (ret < 0) {
        return ret;
    }
    cCount += ret;
    return cCount;
}

int cominitErrPrintFFL(const char *file, const char *func, int line, const char *format, ...) {
    int ret = 0;
    int cCount = 0;
    va_list args;
    ret = fprintf(stderr, COMINIT_PRINT_PREFIX "(%s:%s:%d) Error: ", file, func, line);
    if (ret < 0) {
        return ret;
    }
    cCount += ret;
    va_start(args, format);
    ret = vfprintf(stderr, format, args);
    if (ret < 0) {
        va_end(args);
        return ret;
    }
    va_end(args);
    cCount += ret;
    ret = fprintf(stderr, "\n");
    if (ret < 0) {
        return ret;
    }
    cCount += ret;
    return cCount;
}

int cominitErrnoPrintFFL(const char *file, const char *func, int line, const char *format, ...) {
    int ret = 0;
    int cCount = 0;
    va_list args;
    ret = fprintf(stderr, COMINIT_PRINT_PREFIX "(%s:%s:%d) Error: ", file, func, line);
    if (ret < 0) {
        return ret;
    }
    cCount += ret;
    va_start(args, format);
    ret = vfprintf(stderr, format, args);
    if (ret < 0) {
        va_end(args);
        return ret;
    }
    va_end(args);
    cCount += ret;
    ret = fprintf(stderr, " Errno: %s\n", strerror(errno));
    if (ret < 0) {
        return ret;
    }
    cCount += ret;
    return cCount;
}
