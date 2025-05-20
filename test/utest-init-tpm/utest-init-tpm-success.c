// SPDX-License-Identifier: MIT
/**
 * @file utest-init-tpm-success.c
 * @brief Implementation of a success case unit test for cominitInitTpm().
 */
#include <stdlib.h>
#include <tss2/tss2_esys.h>

#include "common.h"
#include "tpm.h"
#include "unit_test.h"
#include "utest-init-tpm.h"

void cominitInitTpmTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitTpmContext_t ctx;

    will_return(__wrap_Tss2_TctiLdr_Initialize, TSS2_RC_SUCCESS);
    will_return(__wrap_Esys_Initialize, TSS2_RC_SUCCESS);
    assert_int_equal(cominitInitTpm(&ctx), 0);
}
