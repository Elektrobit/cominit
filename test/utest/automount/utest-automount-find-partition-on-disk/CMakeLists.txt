# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-automount-find-partition-on-disk
  SOURCES
    utest-automount-find-partition-on-disk.c
    utest-automount-find-partition-on-disk-success.c
    utest-automount-find-partition-on-disk-param-failure.c
    utest-automount-find-partition-on-disk-gpt-is-negative-failure.c
    utest-automount-find-partition-on-disk-gpt-is-zero-failure.c
    utest-automount-find-partition-on-disk-gptEntrySize-is-negative-failure.c
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
    -Wl,--wrap=strcasecmp
    -Wl,--wrap=free
    -Wl,--wrap=ioctl
    -Wl,--wrap=malloc
)
