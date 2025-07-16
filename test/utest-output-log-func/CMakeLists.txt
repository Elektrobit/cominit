# SPDX-License-Identifier: MIT

create_unit_test(
  NAME
    utest-output-log-func
  SOURCES
    utest-output-log-func.c
    utest-output-log-func-success.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  LIBRARIES
    cmocka
)

create_unit_test(
  NAME
    utest-output-log-func-sensitive
  SOURCES
    utest-output-log-func.c
    utest-output-log-func-success.c
    ${PROJECT_SOURCE_DIR}/src/output.c
  DEFINITIONS
    COMINIT_ENABLE_SENSITIVE_LOGGING
  LIBRARIES
    cmocka
)
