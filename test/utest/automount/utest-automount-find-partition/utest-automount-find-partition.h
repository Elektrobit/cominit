// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition.h
 * @brief Header declaring cmocka unit test functions for cominitAutomountFindPartition().
 */
#ifndef __UTEST_AUTOMOUNT_FIND_PARTITION_H__
#define __UTEST_AUTOMOUNT_FIND_PARTITION_H__

#include "automount.h"

/**
 * Unit test for cominitAutomountFindPartition() successful code path.
 * @param state
 */
void cominitAutomountFindPartitionTestSuccess(void **state);

/**
 * Unit test for cominitAutomountFindPartition() if parameters are not initialized.
 * @param state
 */
void cominitAutomountFindPartitionTestParamFailure(void **state);

#endif /* __UTEST_AUTOMOUNT_FIND_PARTITION_H__ */
