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
 * Unit test that simulates different non integer parameters
 * @param state
 */
void cominitTpmParsePcrIndexTestFailure(void **state);

#endif /* __UTEST_TPM_PARSE_PCR_INDEX_H__ */
