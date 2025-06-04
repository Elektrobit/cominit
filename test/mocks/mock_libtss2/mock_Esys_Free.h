// SPDX-License-Identifier: MIT
/**
 * @file mock_Esys_Free.h
 * @brief Header declaring a mock function for Esys_Free().
 */
#ifndef __MOCK_Esys_Free_H__
#define __MOCK_Esys_Free_H__

#include <tss2/tss2_common.h>
#include <tss2/tss2_esys.h>

/**
 * Mock function for Esys_Free().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
void __wrap_Esys_Free(void *__ptr);

#endif /* __MOCK_Esys_Free_H__ */
