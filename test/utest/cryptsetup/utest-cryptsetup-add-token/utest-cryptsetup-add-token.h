// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-add-token.h
 * @brief Header declaring cmocka unit test functions for cominitCryptsetupAddToken().
 */
#ifndef __UTEST_CRYPTSETUP_OPEN_LUKS_VOLUME_H__
#define __UTEST_CRYPTSETUP_OPEN_LUKS_VOLUME_H__

#include "common.h"
#include "cryptsetup.h"

/**
 * Unit test for cominitCryptsetupAddToken() successful code path.
 * @param state
 */
void cominitCryptsetupAddTokenTestSuccess(void **state);

/**
 * Unit test for cominitCryptsetupAddToken() if parameters are not initialized.
 * @param state
 */
void cominitCryptsetupAddTokenTestParamFailure(void **state);

#endif /* __UTEST_CRYPTSETUP_OPEN_LUKS_VOLUME_H__ */
