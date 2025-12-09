# SPDX-License-Identifier: MIT
if(USE_TPM)
  create_unit_test(
    NAME
      utest-cryptsetup-add-token
    SOURCES
      utest-cryptsetup-add-token.c
      utest-cryptsetup-add-token-success.c
      utest-cryptsetup-add-token-param-failure.c
      ${PROJECT_SOURCE_DIR}/src/cryptsetup.c
      ${PROJECT_SOURCE_DIR}/src/output.c
    DEFINITIONS
      COMINIT_USE_TPM
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
endif()
