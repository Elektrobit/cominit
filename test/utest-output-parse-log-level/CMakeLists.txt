# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-output-parse-log-level
  SOURCES
    utest-output-parse-log-level.c
    utest-output-parse-log-level-success.c
    utest-output-parse-log-level-failure.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    cmocka
)