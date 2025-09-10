// SPDX-License-Identifier: MIT
/**
 * @file mock_memcpy.h
 * @brief Header declaring a mock function for memcpy().
 */
#ifndef __MOCK_MEMCPY_H__
#define __MOCK_MEMCPY_H__

#include <string.h>

/**
 * Mock function for memcpy().
 *
 * Checks that the right parameters are given and return a preset pointer.
 */
int __wrap_memcpy(void *dest, const void *src, size_t n);  // NOLINT(readability-identifier-naming)
                                                           // Rationale: Naming scheme fixed due to linker wrapping.

#endif /* __MOCK_MEMCPY_H__ */
