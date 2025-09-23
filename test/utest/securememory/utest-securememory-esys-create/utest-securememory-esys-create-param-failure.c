// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-create-param-failure.c
 * @brief Implementation of a failure case unit test for cominitSecurememoryEsysCreate().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <tss2/tss2_esys.h>

#include "utest-securememory-esys-create.h"

void cominitSecurememoryEsysCreateTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);

    ESYS_CONTEXT *ectx = calloc(1, sizeof(char));
    ESYS_TR primaryHandle = {0};
    TPM2B_PUBLIC *outPublic = NULL;
    TPM2B_PRIVATE *outPrivate = NULL;
    TPM2B_DIGEST policyDigest = {0};

    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    assert_int_not_equal(cominitSecurememoryEsysCreate(NULL, &primaryHandle, &outPublic, &outPrivate, &policyDigest),
                         0);

    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    assert_int_not_equal(cominitSecurememoryEsysCreate(ectx, NULL, &outPublic, &outPrivate, &policyDigest), 0);

    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    assert_int_not_equal(cominitSecurememoryEsysCreate(ectx, &primaryHandle, NULL, &outPrivate, &policyDigest), 0);

    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    assert_int_not_equal(cominitSecurememoryEsysCreate(ectx, &primaryHandle, &outPublic, NULL, &policyDigest), 0);

    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    assert_int_not_equal(cominitSecurememoryEsysCreate(ectx, &primaryHandle, &outPublic, &outPrivate, NULL), 0);

    free(ectx);
}
