// SPDX-License-Identifier: MIT
/**
 * @file mock_open.h
 * @brief Header declaring a mock function for open().
 */
#ifndef __MOCK_OPEN_H__
#define __MOCK_OPEN_H__

#include <fcntl.h>
#include <stdarg.h>
#include <stdbool.h>
#include <string.h>

/**
 * Mock function for open().
 *
 * If cominitMockCloseEnabled is true then it checks that the right parameters are
 * given.
 * If cominitMockCloseEnabled is false then the call is forwarded to the genuine open
 * method.
 */
int __wrap_open(const char *path, int flags, ...);  // NOLINT(readability-identifier-naming)
                                                    // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Prototype for the genuine open function provided by the linker
 */
int __real_open(const char *path, int flags, ...);  // NOLINT(readability-identifier-naming)
                                                    // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Define if open is used as mock or if open forwards to __real_open.
 * true - mocking enabled , no real open is called
 * false - all calls are forwarded to __real_open aka `open`
 */
extern bool cominitMockOpenEnabled;

#endif /* __MOCK_OPEN_H__ */
