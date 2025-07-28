// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupOpenLuksVolume.h
 * @brief Header declaring a mock function for cominitCryptsetupOpenLuksVolume().
 */
#ifndef __MOCK_COMINIT_CRYPTSETUPOPENLUKSVOLUME_H__
#define __MOCK_COMINIT_CRYPTSETUPOPENLUKSVOLUME_H__

/**
 * Mock function for cominitCryptsetupOpenLuksVolume().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupOpenLuksVolume(char *devCrypt);

#endif /* __MOCK_COMINIT_CRYPTSETUPOPENLUKSVOLUME_H__ */
