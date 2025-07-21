// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSetupDmDeviceCrypt.c
 * @brief Implementation of a mock function for cominitSetupDmDeviceCrypt() using cmocka.
 */
#include "mock_cominitSetupDmDeviceCrypt.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSetupDmDeviceCrypt(char *device, const char *name, const TPM2B_DIGEST *key, uint64_t offsetSectors) {
    check_expected_ptr(device);
    check_expected_ptr(name);
    check_expected_ptr(key);
    check_expected(offsetSectors);

    return mock_type(int);
}
