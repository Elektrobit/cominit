// SPDX-License-Identifier: MIT
/**
 * @file subprocess.h
 * @brief Header related to subprocess implementations
 */

#ifndef __SUBPROCESS_H__
#define __SUBPROCESS_H__

#include <stddef.h>

/**
 * Spawn a subprocess and feed it data on stdin.
 *
 * @param path      Absolute path to the program to execute.
 * @param argv      Null‑terminated array of argument strings; argv[0] should be
 *                  the program name and the last element must be NULL.
 * @param env   The environment for the new process. This must be a NULL‑terminated.
 * @param data      Pointer to the buffer containing data to write to the child’s stdin.
 * @param dataSize   Number of bytes from @p data to write into stdin.
 *
 * @return  0 on success, 1 otherwise
 */
int cominitSubprocessSpawnAndWrite(const char *path, char *const argv[], char *const env[], const void *data,
                                   size_t dataSize);

/**
 * Spawn a subprocess.
 *
 * @param path  Absolute path to the program to execute.
 * @param argv  Null‑terminated array of argument strings; argv[0] should be
 *              the program name and the last element must be NULL.
 * @param env   The environment for the new process. This must be a NULL‑terminated.
 *
 * @return  0 on success, 1 otherwise
 */
int cominitSubprocessSpawn(const char *path, char *const argv[], char *const env[]);

#endif /* __SUBPROCESS_H__ */
