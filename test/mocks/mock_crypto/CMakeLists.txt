# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_crypto
    SOURCES
    mock_cominitCreateSHA256DigestfromKeyfile.c
    mock_cominitCryptoCreatePassphrase.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
