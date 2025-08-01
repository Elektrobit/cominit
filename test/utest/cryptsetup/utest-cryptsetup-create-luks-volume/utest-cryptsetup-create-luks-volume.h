// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-create-luks-volume.h
 * @brief Header declaring cmocka unit test functions for cominitCryptsetupCreateLuksVolume().
 */
#ifndef __UTEST_CRYPTSETUP_CREATE_LUKS_VOLUME_H__
#define __UTEST_CRYPTSETUP_CREATE_LUKS_VOLUME_H__

#include "common.h"
#include "cryptsetup.h"

/**
 * Unit test for cominitCryptsetupCreateLuksVolume() successful code path.
 * @param state
 */
void cominitCryptsetupCreateLuksVolumeTestSuccess(void **state);

/**
 * Unit test for cominitCryptsetupCreateLuksVolume() if parameters are not initialized.
 * @param state
 */
void cominitCryptsetupCreateLuksVolumeTestParamFailure(void **state);

#endif /* __UTEST_CRYPTSETUP_CREATE_LUKS_VOLUME_H__ */
