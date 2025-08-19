# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_libmbedtls
    SOURCES
    mock_mbedtls_pk_free.c
    mock_mbedtls_pk_init.c
    mock_mbedtls_pk_parse_public_keyfile.c
    mock_mbedtls_pk_write_pubkey_der.c
    mock_mbedtls_sha256_ret.c
    mock_mbedtls_sha256.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
