// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-parse-pcr-index.h
 * @brief Header declaring cmocka unit test functions for cominitTpmParsePcrIndex().
 */
#ifndef __UTEST_TPM_PARSE_PCR_INDEX_H__
#define __UTEST_TPM_PARSE_PCR_INDEX_H__

#include <sys/mount.h>

/**
 * Unit test for cominitTpmParsePcrIndex() successful code path.
 * @param state
 */
void cominitTpmParsePcrIndexTestSuccess(void **state);

/**
 * Unit test that simulates a non-numeric index parameter
 * @param state
 */
void cominitTpmParsePcrIndexTestNonIntegerParamFailure(void **state);

/**
 * Unit test that simulates a mixed index parameter (e.g. "3k")
 * @param state
 */
void cominitTpmParsePcrIndexTestMixedIntegerParamFailure(void **state);

/**
 * Unit test that simulates a negative index parameter (e.g. "-3")
 * @param state
 */
void cominitTpmParsePcrIndexTestNegativeIntegerParamFailure(void **state);

/**
 * Unit test that simulates an index parameter that's larger than TPM2_PT_PCR_COUNT
 * @param state
 */
void cominitTpmParsePcrIndexTestTooLargeParamFailure(void **state);

/**
 * Unit test that simulates an index parameter that's equal to TPM2_PT_PCR_COUNT
 * @param state
 */
void cominitTpmParsePcrIndexTestTooLargeParamOnEdgeFailure(void **state);

#endif /* __UTEST_TPM_PARSE_PCR_INDEX_H__ */
