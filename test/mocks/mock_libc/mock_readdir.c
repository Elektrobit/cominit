// SPDX-License-Identifier: MIT
/**
 * @file mock_readdir.c
 * @brief Implementation of a mock function for readdir().
 */
#include "mock_readdir.h"

#include "unit_test.h"

bool cominitMockReaddirEnabled = false;
// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
struct dirent *__wrap_readdir(DIR *dirp) {
    if (cominitMockReaddirEnabled) {
        check_expected_ptr(dirp);

        return mock_type(struct dirent *);
    } else {
        return __real_readdir(dirp);
    }
}
