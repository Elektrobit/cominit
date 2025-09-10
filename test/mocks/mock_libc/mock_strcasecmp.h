// SPDX-License-Identifier: MIT
/**
 * @file mock_strcasecmp.h
 * @brief Header declaring a mock function for strcasecmp().
 */
#ifndef __MOCK_STRCASECMP_H__
#define __MOCK_STRCASECMP_H__

#include <stdbool.h>

/**
 * Mock function for strcasecmp().
 *
 * If cominitMockCloseEnabled is true then it checks that the right parameters are
 * given.
 * If cominitMockCloseEnabled is false then the call is forwarded to the genuine strcasecmp
 * method.
 */
int __wrap_strcasecmp(const char *s1, const char *s2);  // NOLINT(readability-identifier-naming)
                                                        // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Prototype for the genuine strcasecmp function provided by the linker
 */
int __real_strcasecmp(const char *s1, const char *s2);  // NOLINT(readability-identifier-naming)
                                                        // Rationale: Naming scheme fixed due to linker wrapping.
/*
 * Define if strcasecmp is used as mock or if strcasecmp forwards to __real_strcasecmp.
 * true - mocking enabled , no real strcasecmp is called
 * false - all calls are forwarded to __real_strcasecmp aka `strcasecmp`
 */
extern bool cominitMockStrcasecmpEnabled;

#endif /* __MOCK_STRCASECMP_H__ */
