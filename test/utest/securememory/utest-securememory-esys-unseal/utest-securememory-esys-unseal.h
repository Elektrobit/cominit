// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-unseal.h
 * @brief Header declaring cmocka unit test functions for cominitSecurememoryEsysUnseal().
 */
#ifndef __UTEST_SECUREMEMORY_ESYS_UNSEAL_H__
#define __UTEST_SECUREMEMORY_ESYS_UNSEAL_H__

#include "securememory.h"

/**
 * Unit test for cominitSecurememoryEsysUnseal() successful code path.
 * @param state
 */
void cominitSecurememoryEsysUnsealTestSuccess(void **state);

/**
 * Unit test for cominitSecurememoryEsysUnseal() if parameters are not initialized.
 * @param state
 */
void cominitSecurememoryEsysUnsealTestParamFailure(void **state);

#endif /* __UTEST_SECUREMEMORY_ESYS_UNSEAL_H__ */
