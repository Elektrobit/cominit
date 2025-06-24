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

create_mock_lib(NAME libmock_libtss2
    SOURCES
    mock_Esys_Finalize.c
    mock_Esys_Initialize.c
    mock_Esys_PCR_Extend.c
    mock_Esys_SelfTest.c
    mock_Tss2_TctiLdr_Finalize.c
    mock_Tss2_TctiLdr_Initialize.c
    mock_Esys_Free.c
    mock_Esys_TR_FromTPMPublic.c
    mock_Esys_Load.c
    mock_Esys_PolicyPCR.c
    mock_Esys_StartAuthSession.c
    mock_Esys_Unseal.c
    mock_Esys_FlushContext.c
    mock_Esys_CreatePrimary.c
    mock_Esys_PolicyGetDigest.c
    mock_Esys_Create.c
    mock_Esys_EvictControl.c
    mock_Esys_GetRandom.c
    mock_Esys_Clear.c
    mock_Esys_TR_SetAuth.c
    INCLUDES ${CMAKE_CURRENT_SOURCE_DIR}
)
