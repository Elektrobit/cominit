// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-esys-create-success.c
 * @brief Implementation of a success case unit test for cominitSecurememoryEsysCreate().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>
#include <tss2/tss2_esys.h>

#include "unit_test.h"
#include "utest-securememory-esys-create.h"

static char cominitTestString[] = "secret key";

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mlock(const void *addr, size_t len) {
    COMINIT_PARAM_UNUSED(len);
    assert_non_null(addr);

    TPM2B_SENSITIVE_CREATE *outData = (TPM2B_SENSITIVE_CREATE *)addr;
    strcpy((char *)outData->sensitive.data.buffer, cominitTestString);

    return mock_type(int);
}

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_munlock(const void *addr, size_t len) {
    assert_true(len > 0);
    assert_non_null(addr);

    /* Additional test that buffer has been cleared*/
    TPM2B_SENSITIVE_CREATE *outData = (TPM2B_SENSITIVE_CREATE *)addr;
    assert_string_not_equal((char *)outData->sensitive.data.buffer, cominitTestString);
    assert_memory_is_zeroed(outData->sensitive.data.buffer, ARRAY_SIZE(cominitTestString));

    return mock_type(int);
}

void cominitSecurememoryEsysCreateTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    ESYS_CONTEXT *ectx = calloc(1, sizeof(char));
    ESYS_TR primaryHandle = {0};
    TPM2B_PUBLIC *outPublic = NULL;
    TPM2B_PRIVATE *outPrivate = NULL;
    TPM2B_DIGEST policyDigest = {0};

    will_return(__wrap_mlock, 0);

    expect_string(__wrap_cominitCryptoCreatePassphrase, passphrase, cominitTestString);
    expect_any(__wrap_cominitCryptoCreatePassphrase, passphraseSize);
    will_return(__wrap_cominitCryptoCreatePassphrase, 0);

    expect_value(__wrap_Esys_Create, esysContext, ectx);
    expect_value(__wrap_Esys_Create, parentHandle, primaryHandle);
    expect_any(__wrap_Esys_Create, shandle1);
    expect_any(__wrap_Esys_Create, shandle2);
    expect_any(__wrap_Esys_Create, shandle3);
    expect_any(__wrap_Esys_Create, inSensitive);
    expect_any(__wrap_Esys_Create, inPublic);
    expect_any(__wrap_Esys_Create, outsideInfo);
    expect_any(__wrap_Esys_Create, creationPCR);
    expect_value(__wrap_Esys_Create, outPrivate, &outPrivate);
    expect_value(__wrap_Esys_Create, outPublic, &outPublic);
    expect_any(__wrap_Esys_Create, creationData);
    expect_any(__wrap_Esys_Create, creationHash);
    expect_any(__wrap_Esys_Create, creationTicket);
    will_return(__wrap_Esys_Create, TSS2_RC_SUCCESS);

    will_return(__wrap_munlock, 0);

    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);
    expect_any(__wrap_Esys_Free, __ptr);

    assert_int_equal(cominitSecurememoryEsysCreate(ectx, &primaryHandle, &outPublic, &outPrivate, &policyDigest), 0);

    free(ectx);
}
