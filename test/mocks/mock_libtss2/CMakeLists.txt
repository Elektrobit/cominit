# SPDX-License-Identifier: MIT
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
