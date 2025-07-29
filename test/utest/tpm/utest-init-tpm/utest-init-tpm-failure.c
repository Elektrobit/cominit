// SPDX-License-Identifier: MIT
/**
 * @file utest-init-tpm-failure.c
 * @brief Implementation of failure cases unit tests for cominitInitTpm().
 */
#include <stdlib.h>
#include <tss2/tss2_esys.h>

#include "common.h"
#include "securememory.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-init-tpm.h"

void cominitInitTpmTestNullCtxFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    assert_int_not_equal(cominitInitTpm(NULL), 0);
}

void cominitInitTpmTestTss2InitFailFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    will_return(__wrap_Tss2_TctiLdr_Initialize, TSS2_TCTI_RC_GENERAL_FAILURE);
    assert_int_not_equal(cominitInitTpm(&ctx), 0);
}

void cominitInitTpmTestEsysInitFailFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    will_return(__wrap_Tss2_TctiLdr_Initialize, TSS2_RC_SUCCESS);
    will_return(__wrap_Esys_Initialize, TSS2_ESYS_RC_GENERAL_FAILURE);
    assert_int_not_equal(cominitInitTpm(&ctx), 0);
}
