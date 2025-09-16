// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-on-disk-gptEntrySize-is-negative-failure.c
 * @brief Implementation of a failure case unit test for cominitAutomountFindPartitionOnDisk().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>

#include "common.h"
#include "mock_close.h"
#include "mock_open.h"
#include "utest-automount-find-partition-on-disk.h"

void cominitAutomountFindPartitionOnDiskTestGptEntrySizeIsNegativeFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    int negative = -1;
    cominitGPTDisk_t disk = {.diskName = "dev/sda", .blockSize = negative, .hdr = {{0}}};
    const char guidType[37] = "GUID_TYPE_test";
    char partition[256] = {0};
    size_t partitionSize = sizeof(partition);

    cominitGPTHeader_t *hdr = &disk.hdr;
    memset(&hdr->partitionEntrySize, negative, 4);

    expect_string(__wrap_open, path, disk.diskName);
    expect_any(__wrap_open, flags);
    will_return(__wrap_open, cominitDiskFdFailure);

    expect_value(__wrap_ioctl, fd, cominitDiskFdFailure);

    expect_value(__wrap_close, fd, cominitDiskFdFailure);
    will_return(__wrap_close, 0);

    cominitMockCloseEnabled = true;
    cominitMockOpenEnabled = true;
    assert_int_equal(cominitAutomountFindPartitionOnDisk(&disk, guidType, partition, partitionSize), EXIT_FAILURE);
    cominitMockCloseEnabled = false;
    cominitMockOpenEnabled = false;
}
