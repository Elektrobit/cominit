# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_keyring
    SOURCES
    mock_cominitKeyringAddUserKey.c
    mock_cominitKeyringGetKey.c
    INCLUDES
     ${CMAKE_CURRENT_SOURCE_DIR}
)
