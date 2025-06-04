// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSetupDmDeviceCrypt.h
 * @brief Header declaring a mock function for cominitSetupDmDeviceCrypt().
 */
#ifndef __MOCK_COMINIT_SETUPDMDEVICECRYPT_H__
#define __MOCK_COMINIT_SETUPDMDEVICECRYPT_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for cominitSetupDmDeviceCrypt().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSetupDmDeviceCrypt(char *device, const char *name, const TPM2B_DIGEST *key, uint64_t offsetSectors);

#endif /* __MOCK_COMINIT_SETUPDMDEVICECRYPT_H__ */
