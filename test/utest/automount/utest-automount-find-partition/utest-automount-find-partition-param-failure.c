// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-param-failure.c
 * @brief Implementation of a failure case unit test for cominitAutomountFindPartition().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>

#include "common.h"
#include "utest-automount-find-partition.h"

void cominitAutomountFindPartitionTestParamFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    cominitGPTDisk_t disk = {.diskName = "/dev/sda", .blockSize = 512, .hdr = {{0}}};
    const char guidType[37] = {0};
    char partition[256] = {0};
    size_t partitionSize = sizeof(partition);

    assert_int_equal(cominitAutomountFindPartition(NULL, guidType, partition, partitionSize), EXIT_FAILURE);
    assert_int_equal(cominitAutomountFindPartition(&disk, NULL, partition, partitionSize), EXIT_FAILURE);
    assert_int_equal(cominitAutomountFindPartition(&disk, guidType, NULL, partitionSize), EXIT_FAILURE);
    assert_int_equal(cominitAutomountFindPartition(&disk, guidType, partition, 0), EXIT_FAILURE);
}
