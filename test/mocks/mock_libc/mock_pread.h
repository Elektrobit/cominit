// SPDX-License-Identifier: MIT
/**
 * @file mock_pread.h
 * @brief Header declaring a mock function for pread().
 */
#ifndef __MOCK_PREAD_H__
#define __MOCK_PREAD_H__

#include <unistd.h>

/**
 * Mock function for pread().
 *
 * Checks that the right parameters are given and return a preset pointer.
 */
ssize_t __wrap_pread(int fd, void *buf, size_t count,
                     off_t offset);  // NOLINT(readability-identifier-naming)
                                     // Rationale: Naming scheme fixed due to linker wrapping.

#endif /* __MOCK_PREAD_H__ */
