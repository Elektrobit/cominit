// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSubprocessSpawnAndWrite.h
 * @brief Header declaring a mock function for cominitSubprocessSpawnAndWrite().
 */
#ifndef __MOCK_COMINIT_CRYPTSETUPOPENLUKSVOLUME_H__
#define __MOCK_COMINIT_CRYPTSETUPOPENLUKSVOLUME_H__

#include <stddef.h>

/**
 * Mock function for cominitSubprocessSpawnAndWrite().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSubprocessSpawnAndWrite(const char *path, char *const argv[], char *const env[], const void *data,
                                          size_t dataSize);

#endif /* __MOCK_COMINIT_CRYPTSETUPOPENLUKSVOLUME_H__ */
