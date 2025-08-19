# SPDX-License-Identifier: MIT

create_mock_lib(NAME libmock_cryptsetup
    SOURCES
    mock_cominitCryptsetupCreateLuksVolume.c
    mock_cominitCryptsetupOpenLuksVolume.c
    mock_cominitCryptsetupAddToken.c
    INCLUDES
     ${CMAKE_CURRENT_SOURCE_DIR}
)
