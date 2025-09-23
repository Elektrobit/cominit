// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSubprocessSpawn.c
 * @brief Implementation of a mock function for cominitSubprocessSpawn() using cmocka.
 */
#include "mock_cominitSubprocessSpawn.h"
#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSubprocessSpawnAndWrite(const char *path, char *const argv[], char *const env[], const void *data,
                                          size_t dataSize) {
    check_expected_ptr(path);
    check_expected_ptr(argv);
    check_expected_ptr(env);
    check_expected_ptr(data);
    check_expected(dataSize);

    return mock_type(int);
}
