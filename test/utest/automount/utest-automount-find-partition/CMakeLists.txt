# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-automount-find-partition
  SOURCES
    utest-automount-find-partition.c
    utest-automount-find-partition-success.c
    utest-automount-find-partition-param-failure.c
    ${PROJECT_SOURCE_DIR}/src/automount.c
    ${PROJECT_SOURCE_DIR}/src/output.c
    ${PROJECT_SOURCE_DIR}/src/common.c
  LIBRARIES
    libmock_libc
  INCLUDES
  WRAPS
    -Wl,--wrap=open
    -Wl,--wrap=pread
    -Wl,--wrap=close
    -Wl,--wrap=closedir
    -Wl,--wrap=opendir
    -Wl,--wrap=readdir
    -Wl,--wrap=lstat
    -Wl,--wrap=ioctl
    -Wl,--wrap=strcasecmp
)
