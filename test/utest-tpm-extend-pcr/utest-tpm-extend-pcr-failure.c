// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-extend-pcr-failure.c
 * @brief Implementation of failure cases unit tests for cominitTpmParsePCR().
 */
#include <stdlib.h>
#include <tss2/tss2_esys.h>

#include "common.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-tpm-extend-pcr.h"

void cominitTpmExtendPCRTestNullCtxFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char *keyfile = {"keyfile"};

    assert_int_not_equal(cominitTpmExtendPCR(NULL, keyfile, 0), 0);
}

void cominitTpmExtendPCRTestNullKeyfileFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    assert_int_not_equal(cominitTpmExtendPCR(&ctx, NULL, 0), 0);
}

void cominitTpmExtendPCRTestNegativeIntegerParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;
    ESYS_CONTEXT *esysCtx = calloc(1, sizeof(char));
    ctx.esysCtx = esysCtx;
    char *keyfile = {"keyfile"};

    const int idxOffset = -1;
    const ESYS_TR idx = ESYS_TR_PCR0 + idxOffset;

    expect_string(__wrap_Esys_PCR_Extend, esysContext, esysCtx);
    expect_value(__wrap_Esys_PCR_Extend, pcrHandle, idx);
    expect_value(__wrap_Esys_PCR_Extend, shandle1, ESYS_TR_PASSWORD);
    expect_value(__wrap_Esys_PCR_Extend, shandle2, ESYS_TR_NONE);
    expect_value(__wrap_Esys_PCR_Extend, shandle3, ESYS_TR_NONE);
    will_return(__wrap_Esys_PCR_Extend, TSS2_ESYS_RC_GENERAL_FAILURE);
    assert_int_not_equal(cominitTpmExtendPCR(&ctx, keyfile, idxOffset), 0);

    free(esysCtx);
}

void cominitTpmExtendPCRTestTooLargeParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;
    ESYS_CONTEXT *esysCtx = calloc(1, sizeof(char));
    ctx.esysCtx = esysCtx;
    char *keyfile = {"keyfile"};

    const int idxOffset = TPM2_PT_PCR_COUNT + 1;
    const ESYS_TR idx = ESYS_TR_PCR0 + idxOffset;

    expect_string(__wrap_Esys_PCR_Extend, esysContext, esysCtx);
    expect_value(__wrap_Esys_PCR_Extend, pcrHandle, idx);
    expect_value(__wrap_Esys_PCR_Extend, shandle1, ESYS_TR_PASSWORD);
    expect_value(__wrap_Esys_PCR_Extend, shandle2, ESYS_TR_NONE);
    expect_value(__wrap_Esys_PCR_Extend, shandle3, ESYS_TR_NONE);
    will_return(__wrap_Esys_PCR_Extend, TSS2_ESYS_RC_GENERAL_FAILURE);
    assert_int_not_equal(cominitTpmExtendPCR(&ctx, keyfile, idxOffset), 0);

    free(esysCtx);
}

void cominitTpmExtendPCRTestTooLargeParamOnEdgeFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;
    ESYS_CONTEXT *esysCtx = calloc(1, sizeof(char));
    ctx.esysCtx = esysCtx;
    char *keyfile = {"keyfile"};

    const int idxOffset = TPM2_PT_PCR_COUNT;
    const ESYS_TR idx = ESYS_TR_PCR0 + idxOffset;

    expect_string(__wrap_Esys_PCR_Extend, esysContext, esysCtx);
    expect_value(__wrap_Esys_PCR_Extend, pcrHandle, idx);
    expect_value(__wrap_Esys_PCR_Extend, shandle1, ESYS_TR_PASSWORD);
    expect_value(__wrap_Esys_PCR_Extend, shandle2, ESYS_TR_NONE);
    expect_value(__wrap_Esys_PCR_Extend, shandle3, ESYS_TR_NONE);
    will_return(__wrap_Esys_PCR_Extend, TSS2_ESYS_RC_GENERAL_FAILURE);
    assert_int_not_equal(cominitTpmExtendPCR(&ctx, keyfile, idxOffset), 0);

    free(esysCtx);
}
