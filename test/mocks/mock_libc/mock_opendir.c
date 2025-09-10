// SPDX-License-Identifier: MIT
/**
 * @file mock_opendir.c
 * @brief Implementation of a mock function for opendir().
 */
#include "mock_opendir.h"

#include "unit_test.h"

bool cominitMockOpendirEnabled = false;
// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
DIR *__wrap_opendir(const char *name) {
    if (cominitMockOpendirEnabled) {
        check_expected_ptr(name);

        return mock_ptr_type(DIR *);
    } else {
        return __real_opendir(name);
    }
}
