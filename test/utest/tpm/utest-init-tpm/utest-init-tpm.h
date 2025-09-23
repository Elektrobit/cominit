// SPDX-License-Identifier: MIT
/**
 * @file utest-init-tpm.h
 * @brief Header declaring cmocka unit test functions for cominitInitTpm().
 */
#ifndef __UTEST_INIT_TPM_H__
#define __UTEST_INIT_TPM_H__

#include <sys/mount.h>

/**
 * Unit test for cominitInitTpm() successful code path.
 * @param state
 */
void cominitInitTpmTestSuccess(void **state);

/**
 * Unit test that simulates a null value parameter for tpmCtx
 * @param state
 */
void cominitInitTpmTestNullCtxFailure(void **state);

/**
 * Unit test that simulates a Tss2_TctiLdr_Initialize() failure
 * @param state
 */
void cominitInitTpmTestTss2InitFailFailure(void **state);

/**
 * Unit test that simulates a Esys_Initialize() failure
 * @param state
 */
void cominitInitTpmTestEsysInitFailFailure(void **state);

#endif /* __UTEST_TPM_EXTEND_PCR_H__ */
