// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-extend-pcr.h
 * @brief Header declaring cmocka unit test functions for cominitTpmExtendPCR().
 */
#ifndef __UTEST_TPM_EXTEND_PCR_H__
#define __UTEST_TPM_EXTEND_PCR_H__

#include <sys/mount.h>

/**
 * Unit test for cominitTpmExtendPCR() successful code path.
 * @param state
 */
void cominitTpmExtendPCRTestIdx0Success(void **state);

/**
 * Unit test for cominitTpmExtendPCR() successful code path.
 * @param state
 */
void cominitTpmExtendPCRTestIdx2Success(void **state);

/**
 * Unit test that simulates a null value parameter for tpmCtx
 * @param state
 */
void cominitTpmExtendPCRTestNullCtxFailure(void **state);

/**
 * Unit test that simulates a null value parameter for keyfile
 * @param state
 */
void cominitTpmExtendPCRTestNullKeyfileFailure(void **state);

/**
 * Unit test that simulates a negative index parameter (e.g. "-3")
 * @param state
 */
void cominitTpmExtendPCRTestNegativeIntegerParamFailure(void **state);

/**
 * Unit test that simulates an index parameter that's larger than TPM2_PT_PCR_COUNT
 * @param state
 */
void cominitTpmExtendPCRTestTooLargeParamFailure(void **state);

/**
 * Unit test that simulates an index parameter that's equal to TPM2_PT_PCR_COUNT
 * @param state
 */
void cominitTpmExtendPCRTestTooLargeParamOnEdgeFailure(void **state);

#endif /* __UTEST_TPM_EXTEND_PCR_H__ */
