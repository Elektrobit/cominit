// SPDX-License-Identifier: MIT
/**
 * @file utest-tpm-extend-pcr-success.c
 * @brief Implementation of a success case unit test for cominitTpmParsePCR().
 */
#include <stdlib.h>
#include <tss2/tss2_esys.h>

#include "common.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-tpm-extend-pcr.h"

void cominitTpmExtendPCRTestIdx0Success(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;
    ESYS_CONTEXT *esysCtx = calloc(1, sizeof(char));
    ctx.esysCtx = esysCtx;
    char *keyfile = {"keyfile"};

    const int idxOffset = 0;
    const ESYS_TR idx = ESYS_TR_PCR0 + idxOffset;

    expect_string(__wrap_Esys_PCR_Extend, esysContext, esysCtx);
    expect_value(__wrap_Esys_PCR_Extend, pcrHandle, idx);
    expect_value(__wrap_Esys_PCR_Extend, shandle1, ESYS_TR_PASSWORD);
    expect_value(__wrap_Esys_PCR_Extend, shandle2, ESYS_TR_NONE);
    expect_value(__wrap_Esys_PCR_Extend, shandle3, ESYS_TR_NONE);
    will_return(__wrap_Esys_PCR_Extend, TSS2_RC_SUCCESS);
    assert_int_equal(cominitTpmExtendPCR(&ctx, keyfile, idxOffset), 0);

    free(esysCtx);
}

void cominitTpmExtendPCRTestIdx2Success(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;
    ESYS_CONTEXT *esysCtx = calloc(1, sizeof(char));
    ctx.esysCtx = esysCtx;
    char *keyfile = {"keyfile"};

    const int idxOffset = 2;
    const ESYS_TR idx = ESYS_TR_PCR0 + idxOffset;

    expect_string(__wrap_Esys_PCR_Extend, esysContext, esysCtx);
    expect_value(__wrap_Esys_PCR_Extend, pcrHandle, idx);
    expect_value(__wrap_Esys_PCR_Extend, shandle1, ESYS_TR_PASSWORD);
    expect_value(__wrap_Esys_PCR_Extend, shandle2, ESYS_TR_NONE);
    expect_value(__wrap_Esys_PCR_Extend, shandle3, ESYS_TR_NONE);
    will_return(__wrap_Esys_PCR_Extend, TSS2_RC_SUCCESS);
    assert_int_equal(cominitTpmExtendPCR(&ctx, keyfile, idxOffset), 0);

    free(esysCtx);
}
