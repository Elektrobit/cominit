// SPDX-License-Identifier: MIT
/**
 * @file mock_mount.h
 * @brief Header declaring a mock function for mount().
 */
#ifndef __MOCK_MOUNT_H__
#define __MOCK_MOUNT_H__

/**
 * Mock function for mount().
 *
 * Implemented using cmocka. Inputs may be checked and return code set using cmocka API. Otherwise the function is a
 * no-op.
 */
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_mount(const char *source, const char *target, const char *fileSystemType, unsigned long mountFlags,
                 const void *data);
#endif /* __MOCK_MOUNT_H__ */
