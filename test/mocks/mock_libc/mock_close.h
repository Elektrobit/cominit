// SPDX-License-Identifier: MIT
/**
 * @file mock_close.h
 * @brief Header declaring a mock function for close().
 */
#ifndef __MOCK_CLOSE_H__
#define __MOCK_CLOSE_H__

#include <stdbool.h>

/**
 * Mock function for close().
 *
 * If cominitMockCloseEnabled is true then it checks that the right parameters are
 * given.
 * If cominitMockCloseEnabled is false then the call is forwarded to the genuine close
 * method.
 */
int __wrap_close(int fd);  // NOLINT(readability-identifier-naming)
                           // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Prototype for the genuine close function provided by the linker
 */
int __real_close(int fd);  // NOLINT(readability-identifier-naming)
                           // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Define if close is used as mock or if close forwards to __real_close.
 * true - mocking enabled , no real close is called
 * false - all calls are forwarded to __real_close aka `close`
 */
extern bool cominitMockCloseEnabled;

#endif /* __MOCK_CLOSE_H__ */
