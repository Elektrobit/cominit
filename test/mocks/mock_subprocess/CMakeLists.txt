# SPDX-License-Identifier: MIT
create_mock_lib(NAME libmock_subprocess
    SOURCES
    mock_cominitSubprocessSpawnAndWrite.c
    mock_cominitSubprocessSpawn.c
    INCLUDES
     ${CMAKE_CURRENT_SOURCE_DIR}
)
