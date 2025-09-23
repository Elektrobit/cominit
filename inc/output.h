// SPDX-License-Identifier: MIT
/**
 * @file output.h
 * @brief Header related to debug/log output.
 */
#ifndef __OUTPUT_H__
#define __OUTPUT_H__

#include <stdbool.h>

/**
 * Structure defining the different lof levels.
 */
typedef enum {
    COMINIT_LOG_LEVEL_NONE = 0,   ///< No print outs.
    COMINIT_LOG_LEVEL_ERR,        ///< Only error print outs.
    COMINIT_LOG_LEVEL_WARN,       ///< Only error and warning print outs.
    COMINIT_LOG_LEVEL_INFO,       ///< Only error, warning and info print outs.
    COMINIT_LOG_LEVEL_DEBUG,      ///< Only error, warning, info and debug print outs.
    COMINIT_LOG_LEVEL_SENSITIVE,  ///< Only error, warning, info, debug and sensitive print outs.
    COMINIT_LOG_LEVEL_COUNT,      ///< The Number of log levels.
    COMINIT_LOG_LEVEL_INVALID     ///< Invalid log level configuration.
} cominitLogLevelE_t;

/**
 * Prefix to put in front of log/info/error messages.
 */
#define COMINIT_PRINT_PREFIX "[COMINIT] "

/**
 * Print a message. Message is only printed if current visible log level is higher than the message's log level.
 * Sensitive messages can only be printed by setting compiler option.
 *
 * Can be used like printf(). In contrast to printf(), this function adds #COMINIT_PRINT_PREFIX and the given
 * \a file, \a func, and \a line parameters at the start as well as a newline at the end. It uses stderr as its output
 * stream.
 *
 * @return The number of characters printed.
 */
int cominitOutputLogFunc(cominitLogLevelE_t logLevel, const char *file, const char *func, int line, bool printErrno,
                         const char *format, ...);

/**
 * Sets the visible log level.
 *
 * @param cominitLogLevel   The visible log level.
 */
void cominitOutputSetVisibleLogLevel(cominitLogLevelE_t cominitLogLevel);

/**
 * Parses the log level from argv that should be visible.
 *
 *
 * @param logLevel  Pointer to the structure that receives the parsed log level.
 * @param argValue  The parsed value of the argument found in the provided argument vector.
 * @return  EXIT_SUCCESS on success, EXIT_FAILURE otherwise
 */
int cominitOutputParseLogLevel(cominitLogLevelE_t *logLevel, const char *argValue);

#define cominitDebugPrint(...) \
    cominitOutputLogFunc(COMINIT_LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, false, __VA_ARGS__)

#define cominitErrPrint(...) \
    cominitOutputLogFunc(COMINIT_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, false, __VA_ARGS__)

#define cominitErrnoPrint(...) \
    cominitOutputLogFunc(COMINIT_LOG_LEVEL_ERR, __FILE__, __func__, __LINE__, true, __VA_ARGS__)

#define cominitInfoPrint(...) \
    cominitOutputLogFunc(COMINIT_LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, false, __VA_ARGS__)

#define cominitSensitivePrint(...) \
    cominitOutputLogFunc(COMINIT_LOG_LEVEL_SENSITIVE, __FILE__, __func__, __LINE__, false, __VA_ARGS__)

#endif /* __OUTPUT_H__ */
