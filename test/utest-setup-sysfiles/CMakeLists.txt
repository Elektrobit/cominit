# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    utest-setup-sysfiles
  SOURCES
    utest-setup-sysfiles.c
    utest-setup-sysfiles-success.c
    utest-setup-sysfiles-mount-error.c
    utest-setup-sysfiles-mkdir-error.c
    ${PROJECT_SOURCE_DIR}/src/minsetup.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    libmockfunctions
  WRAPS
    -Wl,--wrap=mount
    -Wl,--wrap=mkdir
)
