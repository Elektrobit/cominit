# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-cryptsetup-create-luks-volume
  SOURCES
    utest-cryptsetup-create-luks-volume.c
    utest-cryptsetup-create-luks-volume-success.c
    utest-cryptsetup-create-luks-volume-param-failure.c
    ${PROJECT_SOURCE_DIR}/src/cryptsetup.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    libmock_subprocess
  INCLUDES
    ${TSS2_ESYS_INCLUDE_DIRS}
  DEFINITIONS
    COMINIT_CRYPTSETUP_DIR="test"
  WRAPS
    -Wl,--wrap=cominitSubprocessSpawn
    -Wl,--wrap=cominitSubprocessSpawnAndWrite

)
