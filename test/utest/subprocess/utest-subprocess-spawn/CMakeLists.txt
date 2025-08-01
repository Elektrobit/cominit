# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-subprocess-spawn
  SOURCES
    utest-subprocess-spawn.c
    utest-subprocess-spawn-success.c
    utest-subprocess-spawn-param-failure.c
    ${PROJECT_SOURCE_DIR}/src/subprocess.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    cmocka
  WRAPS
)
