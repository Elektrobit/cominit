#include "tpm.h"

#include <stdio.h>
#include <stdlib.h>
#include <tss2/tss2_esys.h>
#include <tss2/tss2_tctildr.h>

int cominitTpmSelftest(void) {
    ESYS_CONTEXT *esysContext = NULL;
    TSS2_TCTI_CONTEXT *tctiCtx = NULL;
    int result = EXIT_FAILURE;
    const char *tctiConf = "device:/dev/tpm0";

    TSS2_RC rc = Tss2_TctiLdr_Initialize(tctiConf, &tctiCtx);
    if (rc != TSS2_RC_SUCCESS) {
        printf("Error initializing TCTI context");
    } else {
        rc = Esys_Initialize(&esysContext, tctiCtx, NULL);
        if (rc != TSS2_RC_SUCCESS) {
            printf("Error initializing ESYS context");
        } else {
            rc = Esys_SelfTest(esysContext, ESYS_TR_NONE, ESYS_TR_NONE, ESYS_TR_NONE, TPM2_YES);
            if (rc != TSS2_RC_SUCCESS) {
                printf("self test failed");
            } else {
                result = EXIT_SUCCESS;
            }
        }
    }

    Esys_Finalize(&esysContext);
    Tss2_TctiLdr_Finalize(&tctiCtx);

    return result;
}
