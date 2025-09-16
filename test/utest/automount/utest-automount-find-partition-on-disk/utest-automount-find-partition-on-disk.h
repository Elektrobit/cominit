// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-on-disk.h
 * @brief Header declaring cmocka unit test functions for cominitAutomountFindPartitionOnDisk().
 */
#ifndef __UTEST_AUTOMOUNT_FIND_PARTITION_ON_DISK_H__
#define __UTEST_AUTOMOUNT_FIND_PARTITION_ON_DISK_H__

#include <linux/fs.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "automount.h"

extern int cominitDiskFd;
extern int cominitDiskFdFailure;

extern bool cominitMockPreadEnabled;
ssize_t __real_pread(int fd, void *buf, size_t count, off_t offset);  // NOLINT(readability-identifier-naming)
ssize_t __wrap_pread(int fd, void *buf, size_t count, off_t offset);

/**
 * Unit test for cominitAutomountFindPartitionOnDisk() successful code path.
 * @param state
 */
void cominitAutomountFindPartitionOnDiskTestSuccess(void **state);

/**
 * Unit test for cominitAutomountFindPartitionOnDisk() if parameters are not initialized.
 * @param state
 */
void cominitAutomountFindPartitionOnDiskTestParamFailure(void **state);

/**
 * Unit test for cominitAutomountFindPartitionOnDisk() if GPT Header is set to negative values.
 * @param state
 */
void cominitAutomountFindPartitionOnDiskTestGptIsNegativeFailure(void **state);
/**
 * Unit test for cominitAutomountFindPartitionOnDisk() if GPT Header is set to zero.
 * @param state
 */
void cominitAutomountFindPartitionOnDiskTestGptIsZeroFailure(void **state);
/**
 * Unit test for cominitAutomountFindPartitionOnDisk() if the entry size of the GPT Header is set to a negative value.
 * @param state
 */
void cominitAutomountFindPartitionOnDiskTestGptEntrySizeIsNegativeFailure(void **state);

#endif /* __UTEST_AUTOMOUNT_FIND_PARTITION_ON_DISK_H__ */
