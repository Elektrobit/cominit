// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-on-disk-success.c
 * @brief Implementation of a success case unit test for cominitAutomountFindPartitionOnDisk().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <dirent.h>
#include <linux/fs.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "common.h"
#include "mock_close.h"
#include "mock_open.h"
#include "mock_strcasecmp.h"
#include "unit_test.h"
#include "utest-automount-find-partition-on-disk.h"

#define TEST_DIR_DEV "/dev"
#define TEST_DISK "sda"

void cominitAutomountFindPartitionOnDiskTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    cominitGPTDisk_t disk = {.diskName = TEST_DIR_DEV "/" TEST_DISK, .blockSize = 512, .hdr = {{0}}};
    const char guidType[37] = "GUID_TYPE_test";
    char partition[1024] = {0};
    size_t partitionSize = sizeof(partition);

    cominitGPTHeader_t *hdr = &disk.hdr;
    memset(&hdr->partitionEntriesLba, 1, 1);
    memset(&hdr->partitionEntrySize, GPT_HEADER_DEFAULT_ENTRY_SIZE, 1);
    memset(&hdr->partitionEntryCount, 1, 1);

    expect_string(__wrap_open, path, disk.diskName);
    expect_any(__wrap_open, flags);
    will_return(__wrap_open, cominitDiskFd);

    expect_value(__wrap_ioctl, fd, cominitDiskFd);

    expect_value(__wrap_pread, fd, cominitDiskFd);

    expect_string(__wrap_strcasecmp, s2, guidType);
    expect_any(__wrap_strcasecmp, s1);
    will_return(__wrap_strcasecmp, 0);

    expect_value(__wrap_close, fd, cominitDiskFd);
    will_return(__wrap_close, 0);

    cominitMockCloseEnabled = true;
    cominitMockOpenEnabled = true;
    cominitMockPreadEnabled = true;
    cominitMockStrcasecmpEnabled = true;
    assert_int_equal(cominitAutomountFindPartitionOnDisk(&disk, guidType, partition, partitionSize), EXIT_SUCCESS);
    cominitMockPreadEnabled = false;
    cominitMockCloseEnabled = false;
    cominitMockOpenEnabled = false;
    cominitMockStrcasecmpEnabled = false;
}
