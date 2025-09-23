// SPDX-License-Identifier: MIT
/**
 * @file mock_strcasecmp.c
 * @brief Implementation of a mock function for strcasecmp().
 */
#include "mock_strcasecmp.h"

#include "unit_test.h"

bool cominitMockStrcasecmpEnabled = false;
// Rationale: Naming scheme fixed due to linker wrapping.
// NOLINTNEXTLINE(readability-identifier-naming)
int __wrap_strcasecmp(const char *s1, const char *s2) {
    if (cominitMockStrcasecmpEnabled) {
        check_expected_ptr(s1);
        check_expected_ptr(s2);

        return mock_type(int);
    } else {
        return __real_strcasecmp(s1, s2);
    }
}
