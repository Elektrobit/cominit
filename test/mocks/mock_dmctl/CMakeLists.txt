# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_dmctl
    SOURCES
    mock_cominitSetupDmDeviceCrypt.c
    mock_cominitSetupDmDevice.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
    INCLUDES libmock_dmctl
)
