# SPDX-License-Identifier: MIT

if(NOT USE_TPM)
    message(STATUS "USE_TPM is off: skipping test")
    return()
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(TSS2_ESYS REQUIRED tss2-esys)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TSS2_TCTILDR REQUIRED tss2-tctildr)

create_unit_test(
  NAME
    utest-tpm-extend-pcr
  SOURCES
    utest-tpm-extend-pcr.c
    utest-tpm-extend-pcr-failure.c
    utest-tpm-extend-pcr-success.c
    ${PROJECT_SOURCE_DIR}/src/tpm.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  INCLUDES
    ${TSS2_ESYS_INCLUDE_DIRS}
    ${TSS2_TCTILDR_INCLUDE_DIRS}
  LIBRARIES
    libmock_dmctl
    libmock_libc
    libmock_libmbedtls
    libmock_libtss2
  WRAPS
  -Wl,--wrap=Tss2_TctiLdr_Initialize
  -Wl,--wrap=Tss2_TctiLdr_Finalize
  -Wl,--wrap=Esys_SelfTest
  -Wl,--wrap=Esys_Initialize
  -Wl,--wrap=Esys_PCR_Extend
  -Wl,--wrap=Esys_Finalize
  -Wl,--wrap=Esys_Free
  -Wl,--wrap=Esys_TR_FromTPMPublic
  -Wl,--wrap=Esys_Load
  -Wl,--wrap=Esys_PolicyPCR
  -Wl,--wrap=Esys_StartAuthSession
  -Wl,--wrap=Esys_Unseal
  -Wl,--wrap=Esys_FlushContext
  -Wl,--wrap=Esys_CreatePrimary
  -Wl,--wrap=Esys_PolicyGetDigest
  -Wl,--wrap=Esys_Create
  -Wl,--wrap=Esys_EvictControl
  -Wl,--wrap=Esys_GetRandom
  -Wl,--wrap=Esys_Clear
  -Wl,--wrap=Esys_TR_SetAuth
  -Wl,--wrap=mbedtls_pk_free
  -Wl,--wrap=mbedtls_pk_init
  -Wl,--wrap=mbedtls_pk_parse_public_keyfile
  -Wl,--wrap=mbedtls_pk_write_pubkey_der
  -Wl,--wrap=mbedtls_sha256_ret
  -Wl,--wrap=mbedtls_sha256
  -Wl,--wrap=cominitSetupDmDeviceCrypt
)
