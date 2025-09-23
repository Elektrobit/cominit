# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-subprocess-spawn-and-write
  SOURCES
    utest-subprocess-spawn-and-write.c
    utest-subprocess-spawn-and-write-success.c
    utest-subprocess-spawn-and-write-param-failure.c
    ${PROJECT_SOURCE_DIR}/src/subprocess.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    cmocka
  WRAPS
)
