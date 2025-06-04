// SPDX-License-Identifier: MIT
/**
 * @file mock_umount2.h
 * @brief Header declaring a mock function for umount2().
 */
#ifndef __MOCK_UMOUNT2_H__
#define __MOCK_UMOUNT2_H__

/**
 * Mock function for umount2().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
int __wrap_umount2(const char *target, int flags);  // NOLINT(readability-identifier-naming)
                                                    //     Rationale: Naming scheme fixed due to linker wrapping.

#endif /* __MOCK_UMOUNT2_H__ */
