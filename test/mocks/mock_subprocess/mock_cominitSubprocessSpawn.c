// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSubprocessSpawn.c
 * @brief Implementation of a mock function for cominitSubprocessSpawn() using cmocka.
 */
#include "mock_cominitSubprocessSpawn.h"

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSubprocessSpawn(const char *path, char *const argv[], char *const env[]) {
    check_expected_ptr(path);
    check_expected_ptr(argv);
    check_expected_ptr(env);

    return mock_type(int);
}
