// SPDX-License-Identifier: MIT
/**
 * @file utest-delete-tpm.h
 * @brief Header declaring cmocka unit test functions for cominitDeleteTpm().
 */
#ifndef __UTEST_DELETE_TPM_H__
#define __UTEST_DELETE_TPM_H__

#include <sys/mount.h>

/**
 * Unit test for cominitDeleteTpm() successful code path.
 * @param state
 */
void cominitDeleteTpmTestSuccess(void **state);

/**
 * Unit test that simulates a null value parameter for tpmCtx
 * @param state
 */
void cominitDeleteTpmTestNullCtxFailure(void **state);

/**
 * Unit test that simulates a null value parameter for tpmCtx->tctiCtx
 * @param state
 */
void cominitDeleteTpmTestTss2NullFailure(void **state);

/**
 * Unit test that simulates a null value parameter for tpmCtx->esysCtx
 * @param state
 */
void cominitDeleteTpmTestEsysNullFailure(void **state);

#endif /* __UTEST_TPM_EXTEND_PCR_H__ */
