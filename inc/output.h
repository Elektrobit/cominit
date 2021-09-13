// SPDX-License-Identifier: MIT
/**
 * @file output.h
 * @brief Header related to debug/log output.
 */
#ifndef __OUTPUT_H__
#define __OUTPUT_H__

/**
 * Prefix to put in front of log/info/error messages.
 */
#define COMINIT_PRINT_PREFIX "[COMINIT] "

/**
 * Print an info message.
 *
 * Can be used like printf(). In contrast to printf(), this function adds #COMINIT_PRINT_PREFIX at the start and a
 * newline at the end. It uses stdout as its output stream.
 *
 * @return The number of characters printed.
 */
int cominitInfoPrint(const char *format, ...);

/**
 * Macro to print an error message including the offending source file, function, and line using cominitErrPrintFFL().
 */
#define cominitErrPrint(...) cominitErrPrintFFL(__FILE__, __func__, __LINE__, __VA_ARGS__)
/**
 * Print an error message.
 *
 * Can be used like printf(). In contrast to printf(), this function adds #COMINIT_PRINT_PREFIX and the given
 * \a file, \a func, and \a line parameters at the start as well as a newline at the end. It uses stderr as its output
 * stream.
 *
 * The macro cominitErrPrint() should be used to provide the function with the correct \a file, \a func, and \a line
 * parameters.
 *
 * @return The number of characters printed.
 */
int cominitErrPrintFFL(const char *file, const char *func, int line, const char *format, ...);

/**
 * Macro to print an error message using cominitErrnoPrintFFL() including the offending source file, function, and line,
 * as well as the value of errno.
 *
 */
#define cominitErrnoPrint(...) cominitErrnoPrintFFL(__FILE__, __func__, __LINE__, __VA_ARGS__)
/**
 * Print an error message including a text representation of the current value of errno.
 *
 * Can be used like printf(). In contrast to printf(), this function adds #COMINIT_PRINT_PREFIX and the given
 * \a file, \a func, and \a line parameters at the start as well as a text representation of errno and a newline at the
 * end. It uses stderr as its output stream.
 *
 * The macro cominitErrnoPrint() should be used to provide the function with the correct \a file, \a func, and \a line
 * parameters.
 *
 * @return The number of characters printed.
 */
int cominitErrnoPrintFFL(const char *file, const char *func, int line, const char *format, ...);

#endif /* __OUTPUT_H__ */
