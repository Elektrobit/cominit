// SPDX-License-Identifier: MIT
/**
 * @file utest-cryptsetup-open-luks-volume.h
 * @brief Header declaring cmocka unit test functions for cominitCryptsetupOpenLuksVolume().
 */
#ifndef __UTEST_CRYPTSETUP_OPEN_LUKS_VOLUME_H__
#define __UTEST_CRYPTSETUP_OPEN_LUKS_VOLUME_H__

#include "common.h"
#include "cryptsetup.h"

/**
 * Unit test for cominitCryptsetupOpenLuksVolume() successful code path.
 * @param state
 */
void cominitCryptsetupOpenLuksVolumeTestSuccess(void **state);

/**
 * Unit test for cominitCryptsetupOpenLuksVolume() if parameters are not initialized.
 * @param state
 */
void cominitCryptsetupOpenLuksVolumeTestParamFailure(void **state);

#endif /* __UTEST_CRYPTSETUP_OPEN_LUKS_VOLUME_H__ */
