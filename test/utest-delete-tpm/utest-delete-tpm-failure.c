// SPDX-License-Identifier: MIT
/**
 * @file utest-delete-tpm-failure.c
 * @brief Implementation of failure cases unit tests for cominitDeleteTpm().
 */
#include <stdlib.h>
#include <tss2/tss2_esys.h>

#include "common.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-delete-tpm.h"

void cominitDeleteTpmTestNullCtxFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    assert_int_not_equal(cominitDeleteTpm(NULL), 0);
}

void cominitDeleteTpmTestTss2NullFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    ctx.esysCtx = calloc(1, sizeof(char));
    ctx.tctiCtx = NULL;

    assert_int_not_equal(cominitDeleteTpm(&ctx), 0);

    free(ctx.esysCtx);
}

void cominitDeleteTpmTestEsysNullFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    ctx.esysCtx = NULL;
    ctx.tctiCtx = calloc(1, sizeof(char));

    assert_int_not_equal(cominitDeleteTpm(&ctx), 0);

    free(ctx.tctiCtx);
}
