// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-unseal-param-failure.c
 * @brief Implementation of a failure case unit test for cominitSecurememoryEsysUnseal().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>
#include <tss2/tss2_esys.h>

#include "tpm.h"
#include "utest-securememory-esys-unseal.h"

void cominitSecurememoryEsysUnsealTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    ESYS_CONTEXT *ectx = calloc(1, sizeof(char));
    ESYS_TR blobHandle = {0};
    ESYS_TR session = {0};

    assert_int_equal(cominitSecurememoryEsysUnseal(NULL, &blobHandle, &session), TpmFailure);

    assert_int_equal(cominitSecurememoryEsysUnseal(ectx, NULL, &session), TpmFailure);

    assert_int_equal(cominitSecurememoryEsysUnseal(ectx, &blobHandle, NULL), TpmFailure);

    free(ectx);
}
