// SPDX-License-Identifier: MIT
/**
 * @file mock_mount.c
 * @brief Implementation of a mock function for mount() using cmocka.
 */
#include "mock_mount.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mount(const char *source, const char *target, const char *fileSystemType, unsigned long mountFlags,
                 const void *data) {
    check_expected_ptr(source);
    check_expected_ptr(target);
    check_expected_ptr(fileSystemType);
    check_expected(mountFlags);
    check_expected_ptr(data);
    errno = mock_type(int);
    return mock_type(int);
}
