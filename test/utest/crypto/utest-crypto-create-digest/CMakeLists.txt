# SPDX-License-Identifier: MIT

if(USE_TPM)
  find_package(MbedTLS 2.28 REQUIRED)

  create_unit_test(
    NAME
      utest-crypto-create-digest
    SOURCES
      utest-crypto-create-digest.c
      utest-crypto-create-digest-success.c
      utest-crypto-create-digest-failure.c
      ${PROJECT_SOURCE_DIR}/src/crypto.c
      ${PROJECT_SOURCE_DIR}/src/output.c
    DEFINITIONS
      COMINIT_USE_TPM
    INCLUDES
      ${MBEDTLS_INCLUDE_DIR}
    LIBRARIES
      ${MBEDTLS_CRYPTO_LIBRARY}
      cmocka
  )
endif()
