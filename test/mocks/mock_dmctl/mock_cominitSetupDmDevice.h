// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSetupDmDevice.h
 * @brief Header declaring a mock function for cominitSetupDmDevice().
 */
#ifndef __MOCK_COMINIT_SETUPDMDEVICE_H__
#define __MOCK_COMINIT_SETUPDMDEVICE_H__

#include "meta.h"

/**
 * Mock function for cominitSetupDmDevice().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSetupDmDevice(cominitRfsMetaData_t *rfsMeta);

#endif /* __MOCK_COMINIT_SETUPDMDEVICE_H__ */
