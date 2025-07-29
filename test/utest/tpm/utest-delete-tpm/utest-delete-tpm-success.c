// SPDX-License-Identifier: MIT
/**
 * @file utest-delete-tpm-success.c
 * @brief Implementation of a success case unit test for cominitDeleteTpm().
 */
#include <stdlib.h>
#include <tss2/tss2_esys.h>

#include "common.h"
#include "securememory.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-delete-tpm.h"

void cominitDeleteTpmTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    ctx.esysCtx = calloc(1, sizeof(char));
    ctx.tctiCtx = calloc(1, sizeof(char));

    assert_int_equal(cominitDeleteTpm(&ctx), 0);

    free(ctx.esysCtx);
    free(ctx.tctiCtx);
}
