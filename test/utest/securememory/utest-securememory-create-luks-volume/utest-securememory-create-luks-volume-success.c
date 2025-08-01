// SPDX-License-Identifier: MIT
/**
 * @file utest-securememory-create-luks-volume-success.c
 * @brief Implementation of a success case unit test for cominitSecurememoryCreateLuksVolume().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <stdlib.h>
#include <string.h>

#include "unit_test.h"
#include "utest-securememory-create-luks-volume.h"

static char cominitTestString[] = "secret key";

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mlock(const void *addr, size_t len) {
    COMINIT_PARAM_UNUSED(len);
    assert_non_null(addr);

    uint8_t *outData = (uint8_t *)addr;
    memcpy(outData, cominitTestString, ARRAY_SIZE(cominitTestString));

    return mock_type(int);
}

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_munlock(const void *addr, size_t len) {
    assert_true(len > 0);
    assert_non_null(addr);

    /* Additional test that buffer has been cleared*/
    uint8_t *outData = (uint8_t *)addr;
    assert_string_not_equal((char *)outData, cominitTestString);
    assert_memory_is_zeroed(outData, ARRAY_SIZE(cominitTestString));

    return mock_type(int);
}

void cominitSecurememoryCreateLuksVolumeTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);
    char devCryptTest[] = "/dev/crypt";

    will_return(__wrap_mlock, 0);

    expect_string(__wrap_cominitKeyringGetKey, key, cominitTestString);
    expect_any(__wrap_cominitKeyringGetKey, keyDesc);
    expect_any(__wrap_cominitKeyringGetKey, keyMaxLen);
    will_return(__wrap_cominitKeyringGetKey, ARRAY_SIZE(cominitTestString));

    expect_string(__wrap_cominitCryptsetupCreateLuksVolume, passphrase, cominitTestString);
    expect_string(__wrap_cominitCryptsetupCreateLuksVolume, devCrypt, devCryptTest);
    expect_any(__wrap_cominitCryptsetupCreateLuksVolume, passphraseSize);
    will_return(__wrap_cominitCryptsetupCreateLuksVolume, 0);

    will_return(__wrap_munlock, 0);

    assert_int_equal(cominitSecurememoryCreateLuksVolume(devCryptTest), 0);
}
