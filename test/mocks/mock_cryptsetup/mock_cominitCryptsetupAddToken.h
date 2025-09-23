// SPDX-License-Identifier: MIT
/**
 * @file mock_cominitCryptsetupAddToken.h
 * @brief Header declaring a mock function for cominitCryptsetupAddToken().
 */
#ifndef __MOCK_COMINIT_CRYPTSETUPADDTOKEN_H__
#define __MOCK_COMINIT_CRYPTSETUPADDTOKEN_H__

/**
 * Mock function for cominitCryptsetupAddToken().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_cominitCryptsetupAddToken(char *devCrypt);

#endif /* __MOCK_COMINIT_CRYPTSETUPADDTOKEN_H__ */
