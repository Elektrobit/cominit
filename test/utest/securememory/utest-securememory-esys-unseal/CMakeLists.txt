# SPDX-License-Identifier: MIT

find_package(PkgConfig REQUIRED)
pkg_check_modules(TSS2_ESYS REQUIRED tss2-esys)

create_unit_test(
  NAME
    utest-securememory-esys-unseal
  SOURCES
    utest-securememory-esys-unseal.c
    utest-securememory-esys-unseal-success.c
    utest-securememory-esys-unseal-param-failure.c
    ${PROJECT_SOURCE_DIR}/src/securememory.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    libmock_libtss2
    libmock_keyring
    libmock_crypto
    libmock_cryptsetup
  INCLUDES
    ${TSS2_ESYS_INCLUDE_DIRS}
  WRAPS
    -Wl,--wrap=Esys_Unseal
    -Wl,--wrap=cominitKeyringAddUserKey
    -Wl,--wrap=cominitKeyringGetKey
    -Wl,--wrap=Esys_Create
    -Wl,--wrap=Esys_Free
    -Wl,--wrap=cominitCryptoCreatePassphrase
    -Wl,--wrap=cominitCryptsetupCreateLuksVolume
    -Wl,--wrap=mlock
    -Wl,--wrap=munlock
)
