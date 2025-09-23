// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-on-disk-gpt-is-negative-failure.c
 * @brief Implementation of a failure case unit test for cominitAutomountFindPartitionOnDisk().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <string.h>

#include "common.h"
#include "mock_close.h"
#include "mock_open.h"
#include "mock_strcasecmp.h"
#include "utest-automount-find-partition-on-disk.h"

void cominitAutomountFindPartitionOnDiskTestGptIsNegativeFailure(void **state) {
    COMINIT_PARAM_UNUSED(state);
    int negative = -1;
    cominitGPTDisk_t disk = {.diskName = "dev/sda", .blockSize = negative, .hdr = {{0}}};
    const char guidType[37] = "GUID_TYPE_test";
    char partition[256] = {0};
    size_t partitionSize = sizeof(partition);

    cominitGPTHeader_t *hdr = &disk.hdr;
    memset(hdr, negative, sizeof(cominitGPTHeader_t));
    memset(&hdr->partitionEntrySize, 0, 4);
    memset(&hdr->partitionEntrySize, GPT_HEADER_DEFAULT_ENTRY_SIZE, 1);

    expect_string(__wrap_open, path, disk.diskName);
    expect_any(__wrap_open, flags);
    will_return(__wrap_open, cominitDiskFdFailure);

    expect_value(__wrap_ioctl, fd, cominitDiskFdFailure);

    expect_any_always(__wrap_pread, fd);

    expect_any_always(__wrap_strcasecmp, s2);
    expect_any_always(__wrap_strcasecmp, s1);
    will_return_always(__wrap_strcasecmp, -1);

    expect_value(__wrap_close, fd, cominitDiskFdFailure);
    will_return(__wrap_close, 0);

    cominitMockCloseEnabled = true;
    cominitMockOpenEnabled = true;
    cominitMockPreadEnabled = true;
    cominitMockStrcasecmpEnabled = true;
    assert_int_equal(cominitAutomountFindPartitionOnDisk(&disk, guidType, partition, partitionSize), EXIT_FAILURE);
    cominitMockPreadEnabled = false;
    cominitMockCloseEnabled = false;
    cominitMockOpenEnabled = false;
    cominitMockStrcasecmpEnabled = false;
}
