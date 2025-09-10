# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_libc
    SOURCES
    mock_close.c
    mock_closedir.c
    mock_free.c
    mock_lstat.c
    mock_malloc.c
    mock_memcmp.c
    mock_memcpy.c
    mock_open.c
    mock_opendir.c
    mock_pread.c
    mock_readdir.c
    mock_mount.c
    mock_umount2.c
    mock_mkdir.c
    mock_strcasecmp.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
