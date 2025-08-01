// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-create.h
 * @brief Header declaring cmocka unit test functions for cominitSecurememoryEsysCreate().
 */
#ifndef __UTEST_SECUREMEMORY_ESYS_CREATE_H__
#define __UTEST_SECUREMEMORY_ESYS_CREATE_H__

#include "securememory.h"

/**
 * Unit test for cominitSecurememoryEsysCreate() successful code path.
 * @param state
 */
void cominitSecurememoryEsysCreateTestSuccess(void **state);

/**
 * Unit test for cominitSecurememoryEsysCreate() if parameters are not initialized.
 * @param state
 */
void cominitSecurememoryEsysCreateTestParamFailure(void **state);

#endif /* __UTEST_SECUREMEMORY_ESYS_CREATE_H__ */
