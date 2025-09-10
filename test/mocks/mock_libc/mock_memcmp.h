// SPDX-License-Identifier: MIT
/**
 * @file mock_memcmp.h
 * @brief Header declaring a mock function for memcmp().
 */
#ifndef __MOCK_MEMCMP_H__
#define __MOCK_MEMCMP_H__

#include <string.h>

/**
 * Mock function for memcmp().
 *
 * Checks that the right parameters are given and return a preset pointer.
 */
int __wrap_memcmp(const void *s1, const void *s2, size_t n);  // NOLINT(readability-identifier-naming)
                                                              // Rationale: Naming scheme fixed due to linker wrapping.

#endif /* __MOCK_MEMCMP_H__ */
