# SPDX-License-Identifier: MIT

find_package(PkgConfig REQUIRED)
pkg_check_modules(TSS2_ESYS REQUIRED tss2-esys)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TSS2_TCTILDR REQUIRED tss2-tctildr)

create_unit_test(
  NAME
    utest-delete-tpm
  SOURCES
    utest-delete-tpm.c
    utest-delete-tpm-failure.c
    utest-delete-tpm-success.c
    ${PROJECT_SOURCE_DIR}/src/tpm.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  INCLUDES
    ${TSS2_ESYS_INCLUDE_DIRS}
    ${TSS2_TCTILDR_INCLUDE_DIRS}
  LIBRARIES
    libmockfunctions
  WRAPS
    -Wl,--wrap=Tss2_TctiLdr_Initialize
    -Wl,--wrap=Tss2_TctiLdr_Finalize
    -Wl,--wrap=Esys_SelfTest
    -Wl,--wrap=Esys_Initialize
    -Wl,--wrap=Esys_PCR_Extend
    -Wl,--wrap=Esys_Finalize
    -Wl,--wrap=mbedtls_pk_free
    -Wl,--wrap=mbedtls_pk_init
    -Wl,--wrap=mbedtls_pk_parse_public_keyfile
    -Wl,--wrap=mbedtls_pk_write_pubkey_der
    -Wl,--wrap=mbedtls_sha256_ret
)
