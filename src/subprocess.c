// SPDX-License-Identifier: MIT
/**
 * @file subprocess.c
 * @brief Implementation of subprocess handling such as spawn a child proccess.
 */

#include "subprocess.h"

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <sysexits.h>
#include <unistd.h>

#include "output.h"

int cominitSubprocessSpawnAndWrite(const char *path, char *const argv[], char *const env[], const void *data,
                                   size_t dataSize) {
    int result = EXIT_FAILURE;
    int pipefd[2] = {0};

    if (path == NULL || argv == NULL || env == NULL || data == NULL || dataSize == 0) {
        cominitErrPrint("Invalid parameters");
    } else {
        if (pipe(pipefd) == 0) {
            pid_t pid = fork();
            if (pid < 0) {
                close(pipefd[0]);
                close(pipefd[1]);
                cominitErrnoPrint("fork failed");
            }
            if (pid == 0) {
                close(pipefd[1]);
                if (dup2(pipefd[0], STDIN_FILENO) < 0) {
                    _exit(EX_OSERR);
                }
                close(pipefd[0]);

                execve(path, argv, env);
                cominitErrnoPrint("execve failed");
                _exit(EXIT_FAILURE);
            }
            if (pid > 0) {
                close(pipefd[0]);
                ssize_t written = write(pipefd[1], data, dataSize);
                close(pipefd[1]);
                if (written < 0) {
                    cominitErrnoPrint("write failed");
                } else {
                    cominitSensitivePrint("wrote %d bytes to stdin", written);
                    int status = -1;
                    waitpid(pid, &status, 0);
                    if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                        result = EXIT_SUCCESS;
                    } else {
                        cominitErrPrint("child failed or was terminated.");
                    }
                }
            }
        } else {
            cominitErrnoPrint("pipe failed");
        }
    }

    return result;
}

int cominitSubprocessSpawn(const char *path, char *const argv[], char *const env[]) {
    int result = EXIT_FAILURE;

    if (path == NULL || argv == NULL || env == NULL) {
        cominitErrPrint("Invalid parameters");
    } else {
        pid_t pid = fork();
        if (pid < 0) {
            cominitErrnoPrint("fork failed");
        }
        if (pid == 0) {
            execve(path, argv, env);
            cominitErrnoPrint("execve failed");
            _exit(EXIT_FAILURE);
        }
        if (pid > 0) {
            int status = -1;
            waitpid(pid, &status, 0);
            if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
                result = EXIT_SUCCESS;
            } else {
                cominitErrPrint("child failed or was terminated.");
            }
        }
    }

    return result;
}