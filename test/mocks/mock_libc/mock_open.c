// SPDX-License-Identifier: MIT
/**
 * @file mock_open.c
 * @brief Implementation of a mock function for open().
 */
#include "mock_open.h"

#include "unit_test.h"

bool cominitMockOpenEnabled = false;
// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
int __wrap_open(const char *path, int flags, ...) {
    mode_t mode = 0;

    if (flags & O_CREAT) {
        va_list ap;
        va_start(ap, flags);
        mode = va_arg(ap, mode_t);
        va_end(ap);
    }

    if (cominitMockOpenEnabled) {
        check_expected_ptr(path);
        check_expected(flags);
        if (flags & O_CREAT) {
            check_expected(mode);
        }
        return mock_type(int);
    } else {
        if (flags & O_CREAT) {
            return __real_open(path, flags, mode);
        } else {
            return __real_open(path, flags);
        }
    }
}
