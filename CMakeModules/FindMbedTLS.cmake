# SPDX-License-Identifier: MIT
#
# Find mbedTLS library
#
# This will define the variables:
#   MBEDTLS_FOUND
#   MBEDTLS_VERSION
#   MBEDTLS_INCLUDE_DIR
#   MBEDTLS_LIBRARIES
#   MBEDTLS_TLS_LIBRARY
#   MBEDTLS_X509_LIBRARY
#   MBEDTLS_CRYPTO_LIBRARY

# Search for mbedTLS library
find_path(MBEDTLS_INCLUDE_DIR NAMES mbedtls/ssl.h)
find_library(MBEDTLS_TLS_LIBRARY NAMES mbedtls)
find_library(MBEDTLS_X509_LIBRARY NAMES mbedx509)
find_library(MBEDTLS_CRYPTO_LIBRARY NAMES mbedcrypto)
set(MBEDTLS_LIBRARIES "${MBEDTLS_TLS_LIBRARY}" "${MBEDTLS_X509_LIBRARY}" "${MBEDTLS_CRYPTO_LIBRARY}")

if (MBEDTLS_INCLUDE_DIR)
  file(READ ${MBEDTLS_INCLUDE_DIR}/mbedtls/build_info.h MBEDTLS_BUILDINFO)
  string(REGEX MATCH "MBEDTLS_VERSION_STRING +\"[0-9|.]+\"" MBEDTLSMATCH ${MBEDTLS_BUILDINFO})
  if (MBEDTLSMATCH)
    string(REGEX REPLACE "MBEDTLS_VERSION_STRING +\"([0-9|.]+)\"" "\\1" MBEDTLS_VERSION ${MBEDTLSMATCH})
  endif ()
endif ()

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(
  MbedTLS
  FOUND_VAR
    MBEDTLS_FOUND
  REQUIRED_VARS
    MBEDTLS_INCLUDE_DIR
    MBEDTLS_LIBRARIES
    MBEDTLS_TLS_LIBRARY
    MBEDTLS_X509_LIBRARY
    MBEDTLS_CRYPTO_LIBRARY
  VERSION_VAR
    MBEDTLS_VERSION
)

mark_as_advanced(
  MBEDTLS_INCLUDE_DIR
  MBEDTLS_LIBRARIES
  MBEDTLS_TLS_LIBRARY
  MBEDTLS_X509_LIBRARY
  MBEDTLS_CRYPTO_LIBRARY
)
