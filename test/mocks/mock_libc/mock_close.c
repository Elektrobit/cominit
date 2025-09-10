// SPDX-License-Identifier: MIT
/**
 * @file mock_close.c
 * @brief Implementation of a mock function for close().
 */
#include "mock_close.h"

#include "unit_test.h"

bool cominitMockCloseEnabled = false;
// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
int __wrap_close(int fd) {
    if (cominitMockCloseEnabled) {
        check_expected(fd);
        return mock_type(int);
    } else {
        return __real_close(fd);
    }
}
