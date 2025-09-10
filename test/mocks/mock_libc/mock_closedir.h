// SPDX-License-Identifier: MIT
/**
 * @file mock_closedir.h
 * @brief Header declaring a mock function for closedirdir().
 */
#ifndef __MOCK_CLOSEDIR_H__
#define __MOCK_CLOSEDIR_H__

#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * Mock function for closedir().
 *
 * If cominitMockClosedirEnabled is true then it checks that the right parameters are
 * given.
 * If cominitMockClosedirEnabled is false then the call is forwarded to the genuine closedir
 * method.
 */
void __wrap_closedir(DIR *dirp);  // NOLINT(readability-identifier-naming)
                                  // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Prototype for the genuine closedir function provided by the linker
 */
void __real_closedir(DIR *dirp);  // NOLINT(readability-identifier-naming)
                                  // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Define if closedir is used as mock or if closedir forwards to __real_closedir.
 * true - mocking enabled , no real closedir is called
 * false - all calls are forwarded to __real_closedir aka `closedir`
 */
extern bool cominitMockClosedirEnabled;

#endif /* __MOCK_CLOSEDIR_H__ */
