// SPDX-License-Identifier: MIT
/**
 * @file mock_closedir.c
 * @brief Implementation of a mock function for closedir().
 */
#include "mock_closedir.h"

#include "unit_test.h"

bool cominitMockClosedirEnabled = false;
// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
void __wrap_closedir(DIR *dirp) {
    if (cominitMockClosedirEnabled) {
        check_expected_ptr(dirp);
    } else {
        __real_closedir(dirp);
    }
}
