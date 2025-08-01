// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-unseal-success.c
 * @brief Implementation of a success case unit test for cominitSecurememoryEsysUnseal().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>
#include <tss2/tss2_esys.h>

#include "tpm.h"
#include "unit_test.h"
#include "utest-securememory-esys-unseal.h"

static TPM2B_SENSITIVE_DATA *cominitTestOutData = NULL;
static char cominitTestString[] = "secret key";

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
TSS2_RC __wrap_Esys_Unseal(ESYS_CONTEXT *esysContext, ESYS_TR itemHandle, ESYS_TR shandle1, ESYS_TR shandle2,
                           ESYS_TR shandle3, TPM2B_SENSITIVE_DATA **outData) {
    check_expected_ptr(esysContext);
    check_expected(itemHandle);
    check_expected(shandle1);
    check_expected(shandle2);
    check_expected(shandle3);
    check_expected_ptr(outData);

    *outData = calloc(1, sizeof(TPM2B_SENSITIVE_DATA));
    cominitTestOutData = *outData;

    return mock_type(TSS2_RC);
}

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mlock(const void *addr, size_t len) {
    COMINIT_PARAM_UNUSED(len);
    assert_non_null(addr);

    TPM2B_SENSITIVE_DATA *outData = (TPM2B_SENSITIVE_DATA *)addr;
    outData->size = ARRAY_SIZE(cominitTestString);
    strcpy((char *)outData->buffer, cominitTestString);

    return mock_type(int);
}

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_munlock(const void *addr, size_t len) {
    assert_true(len > 0);
    assert_non_null(addr);

    /* Additional test that buffer has been cleared*/
    TPM2B_SENSITIVE_DATA *outData = (TPM2B_SENSITIVE_DATA *)addr;
    assert_string_not_equal((char *)outData->buffer, cominitTestString);
    assert_memory_is_zeroed(outData->buffer, ARRAY_SIZE(cominitTestString));

    return mock_type(int);
}

void cominitSecurememoryEsysUnsealTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    ESYS_CONTEXT *ectx = calloc(1, sizeof(char));
    ESYS_TR blobHandle = {0};
    ESYS_TR session = {0};

    expect_value(__wrap_Esys_Unseal, esysContext, ectx);
    expect_value(__wrap_Esys_Unseal, itemHandle, blobHandle);
    expect_value(__wrap_Esys_Unseal, shandle1, session);
    expect_value(__wrap_Esys_Unseal, shandle2, ESYS_TR_NONE);
    expect_value(__wrap_Esys_Unseal, shandle3, ESYS_TR_NONE);
    expect_any(__wrap_Esys_Unseal, outData);
    will_return(__wrap_Esys_Unseal, 0);

    will_return(__wrap_mlock, 0);

    expect_string(__wrap_cominitKeyringAddUserKey, key, cominitTestString);
    expect_any(__wrap_cominitKeyringAddUserKey, keyDesc);
    expect_any(__wrap_cominitKeyringAddUserKey, keyLen);
    will_return(__wrap_cominitKeyringAddUserKey, 0);

    will_return(__wrap_munlock, 0);

    expect_any(__wrap_Esys_Free, __ptr);

    assert_int_equal(cominitSecurememoryEsysUnseal(ectx, &blobHandle, &session), Unsealed);

    free(ectx);
    free(cominitTestOutData);
}
