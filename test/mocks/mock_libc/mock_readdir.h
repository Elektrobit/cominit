// SPDX-License-Identifier: MIT
/**
 * @file mock_readdir.h
 * @brief Header declaring a mock function for readdir().
 */
#ifndef __MOCK_READDIR_H__
#define __MOCK_READDIR_H__

#include <dirent.h>
#include <stdbool.h>

/**
 * Mock function for readdir().
 *
 * If cominitMockCloseEnabled is true then it checks that the right parameters are
 * given.
 * If cominitMockCloseEnabled is false then the call is forwarded to the genuine readdir
 * method.
 */
struct dirent *__wrap_readdir(DIR *dirp);  // NOLINT(readability-identifier-naming)
                                           // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Prototype for the genuine readdir function provided by the linker
 */
struct dirent *__real_readdir(DIR *dirp);  // NOLINT(readability-identifier-naming)
                                           // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Define if readdir is used as mock or if readdir forwards to __real_readdir.
 * true - mocking enabled , no real readdir is called
 * false - all calls are forwarded to __real_readdir aka `readdir`
 */
extern bool cominitMockReaddirEnabled;

#endif /* __MOCK_READDIR_H__ */
