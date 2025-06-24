// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSetupDmDevice.c
 * @brief Implementation of a mock function for cominitSetupDmDevice() using cmocka.
 */
#include "mock_cominitSetupDmDevice.h"

#include <errno.h>

#include "unit_test.h"

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSetupDmDevice(cominitRfsMetaData_t *rfsMeta) {
    check_expected_ptr(rfsMeta);

    return mock_type(int);
}
