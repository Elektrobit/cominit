// SPDX-License-Identifier: MIT
/**
 * @file mock_pread.c
 * @brief Implementation of a mock function for pread().
 */
#include "mock_pread.h"

#include "unit_test.h"

// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
ssize_t __wrap_pread(int fd, void *buf, size_t count, off_t offset) {
    check_expected(fd);
    check_expected_ptr(buf);
    check_expected(count);
    check_expected(offset);

    return mock_type(ssize_t);
}
