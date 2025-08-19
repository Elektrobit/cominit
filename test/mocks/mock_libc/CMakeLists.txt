# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_libc
    SOURCES
    mock_mount.c
    mock_umount2.c
    mock_mkdir.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
