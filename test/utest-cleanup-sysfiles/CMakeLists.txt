# SPDX-License-Identifier: MIT
create_unit_test(
  NAME
    utest-cleanup-sysfiles
  SOURCES
    utest-cleanup-sysfiles.c
    utest-cleanup-sysfiles-success.c
    utest-cleanup-sysfiles-umount-error.c
    ${PROJECT_SOURCE_DIR}/src/minsetup.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    libmock_dmctl
    libmock_libc    
  WRAPS
    -Wl,--wrap=umount2
    -Wl,--wrap=cominitSetupDmDevice
)
