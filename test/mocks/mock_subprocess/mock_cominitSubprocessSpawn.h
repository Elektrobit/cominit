// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitSubprocessSpawn.h
 * @brief Header declaring a mock function for cominitSubprocessSpawn().
 */
#ifndef __MOCK_COMINIT_SUBPROCESSSPAWN_H__
#define __MOCK_COMINIT_SUBPROCESSSPAWN_H__

/**
 * Mock function for cominitSubprocessSpawn().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitSubprocessSpawn(const char *path, char *const argv[], char *const env[]);

#endif /* __MOCK_COMINIT_SUBPROCESSSPAWN_H__ */
