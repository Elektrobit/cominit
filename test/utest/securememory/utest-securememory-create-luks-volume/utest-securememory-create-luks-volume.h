// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-create-luks-volume.h
 * @brief Header declaring cmocka unit test functions for cominitSecurememoryCreateLuksVolume().
 */
#ifndef __UTEST_SECUREMEMORY_CREATE_LUKS_VOLUME_H__
#define __UTEST_SECUREMEMORY_CREATE_LUKS_VOLUME_H__

#include "securememory.h"

/**
 * Unit test for cominitSecurememoryCreateLuksVolume() successful code path.
 * @param state
 */
void cominitSecurememoryCreateLuksVolumeTestSuccess(void **state);

/**
 * Unit test for cominitSecurememoryCreateLuksVolume() if parameters are not initialized.
 * @param state
 */
void cominitSecurememoryCreateLuksVolumeTestParamFailure(void **state);

#endif /* __UTEST_SECUREMEMORY_CREATE_LUKS_VOLUME_H__ */
