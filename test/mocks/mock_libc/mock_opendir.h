// SPDX-License-Identifier: MIT
/**
 * @file mock_opendir.h
 * @brief Header declaring a mock function for opendir().
 */
#ifndef __MOCK_OPENDIR_H__
#define __MOCK_OPENDIR_H__

#include <dirent.h>
#include <stdbool.h>
#include <sys/types.h>

/**
 * Mock function for opendir().
 *
 * If cominitMockCloseEnabled is true then it checks that the right parameters are
 * given.
 * If cominitMockCloseEnabled is false then the call is forwarded to the genuine opendir
 * method.
 */
DIR *__wrap_opendir(const char *name);  // NOLINT(readability-identifier-naming)
                                        // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Prototype for the genuine opendir function provided by the linker
 */
DIR *__real_opendir(const char *name);  // NOLINT(readability-identifier-naming)
                                        // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Define if opendir is used as mock or if opendir forwards to __real_opendir.
 * true - mocking enabled , no real opendir is called
 * false - all calls are forwarded to __real_opendir aka `opendir`
 */
extern bool cominitMockOpendirEnabled;

#endif /* __MOCK_OPENDIR_H__ */
