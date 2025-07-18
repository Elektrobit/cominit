# SPDX-License-Identifier: MIT
function(create_mock_lib)
  cmake_parse_arguments(PARSED_ARGS "" "NAME" "SOURCES;INCLUDES;LIBRARIES;DEFINITIONS" ${ARGN})
  message("create ${PARSED_ARGS_NAME} ${PARSED_SOURCES}")

  add_library(${PARSED_ARGS_NAME} STATIC ${PARSED_ARGS_SOURCES})

  if(PARSED_ARGS_DEFINITIONS)
    target_compile_definitions(${PARSED_ARGS_NAME} PRIVATE ${PARSED_ARGS_DEFINITIONS})
  endif()

  target_link_libraries(${PARSED_ARGS_NAME} PRIVATE cmocka ${PARSED_ARGS_LIBRARIES})

  target_include_directories(
    ${PARSED_ARGS_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/test
    INTERFACE
    ${PARSED_ARGS_INCLUDES}
  )
endfunction()

create_mock_lib(NAME libmock_crypto
    SOURCES
    mock_cominitCreateSHA256DigestfromKeyfile.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
