# SPDX-License-Identifier: MIT

if(NOT USE_TPM)
    message(STATUS "USE_TPM is off: skipping test")
    return()
endif()

find_package(MbedTLS 2.28 REQUIRED)

create_unit_test(
  NAME
    utest-crypto-verify-signature
  SOURCES
    utest-crypto-verify-signature.c
    utest-crypto-verify-signature-success.c
    utest-crypto-verify-signature-failure.c
    ${PROJECT_SOURCE_DIR}/src/crypto.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  INCLUDES
    ${MBEDTLS_INCLUDE_DIR}
  LIBRARIES
    ${MBEDTLS_CRYPTO_LIBRARY}
    cmocka
)
