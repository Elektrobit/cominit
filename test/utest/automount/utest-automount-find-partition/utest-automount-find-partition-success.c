// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-success.c
 * @brief Implementation of a success case unit test for cominitAutomountFindPartition().
 */

#include <cmocka_extensions/cmocka_extensions.h>
#include <dirent.h>
#include <linux/fs.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#include "common.h"
#include "mock_close.h"
#include "mock_closedir.h"
#include "mock_open.h"
#include "mock_opendir.h"
#include "mock_readdir.h"
#include "mock_strcasecmp.h"
#include "unit_test.h"
#include "utest-automount-find-partition.h"

#if defined(__GLIBC__)
#define IOCTL_REQ_T unsigned long
#else
#define IOCTL_REQ_T int
#endif

#define TEST_DIR_DEV "/dev"
#define TEST_DISK "sdx"

static int cominitBlockDeviceFd = 123;
static int cominitDiskFd = 4711;

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_ioctl(int fd, unsigned long request, ...) {
    check_expected(fd);

    va_list ap;
    va_start(ap, request);

    switch (request) {
        case BLKSSZGET: {
            int *out = va_arg(ap, int *);
            assert_non_null(out);
            *out = 512;
            va_end(ap);
            return 0;
        }
        case BLKGETSIZE64: {
            uint64_t *out = va_arg(ap, uint64_t *);
            assert_non_null(out);
            *out = 1024;
            va_end(ap);
            return 0;
        }
        default:
            va_end(ap);
            return -1;
    }
}

bool cominitMockLstatEnabled = false;
int __real_lstat(const char *restrict path, struct stat *restrict buf);  // NOLINT(readability-identifier-naming)
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_lstat(const char *restrict path, struct stat *restrict buf) {
    if (cominitMockLstatEnabled) {
        check_expected_ptr(path);
        assert_non_null(buf);

        *buf = (struct stat){0};
        buf->st_mode = S_IFBLK | 0600;
        return mock_type(int);
    } else {
        return __real_lstat(path, buf);
    }
}

bool cominitMockPreadEnabled = false;
ssize_t __real_pread(int fd, void *buf, size_t count, off_t offset);  // NOLINT(readability-identifier-naming)
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
ssize_t __wrap_pread(int fd, void *buf, size_t count, off_t offset) {
    if (cominitMockPreadEnabled) {
        COMINIT_PARAM_UNUSED(count);
        COMINIT_PARAM_UNUSED(offset);
        check_expected(fd);
        assert_non_null(buf);
        if (fd == cominitBlockDeviceFd) {
            cominitGPTHeader_t *hdr = buf;
            static const char gptSignature[8] = {'E', 'F', 'I', ' ', 'P', 'A', 'R', 'T'};
            memcpy(hdr->signature, gptSignature, sizeof(gptSignature));
            memset(&hdr->partitionEntriesLba, 1, 1);
            memset(&hdr->partitionEntrySize, GPT_HEADER_DEFAULT_ENTRY_SIZE, 1);
            memset(&hdr->partitionEntryCount, 1, 1);
        }
        if (fd == cominitDiskFd) {
            /*set entry to non zero*/
            memset(buf, 0x11, GPT_HEADER_DEFAULT_ENTRY_SIZE);
        }
        return mock_type(ssize_t);
    } else {
        return __real_pread(fd, buf, count, offset);
    }
}

void cominitAutomountFindPartitionTestSuccess(void **state) {
    COMINIT_PARAM_UNUSED(state);

    cominitGPTDisk_t emptyDisk = {.diskName = "", .blockSize = 0, .hdr = {{0}}};
    const char guidType[37] = "GUID_TYPE_test";
    char partition[1024] = {0};
    size_t partitionSize = sizeof(partition);
    struct dirent deviceEntry = {0};
    strcpy(deviceEntry.d_name, TEST_DISK);
    int dummy;
    DIR *fakeDirPtr = (DIR *)&dummy;

    expect_string(__wrap_opendir, name, TEST_DIR_DEV);
    will_return(__wrap_opendir, fakeDirPtr);

    expect_value(__wrap_readdir, dirp, fakeDirPtr);
    will_return(__wrap_readdir, &deviceEntry);

    expect_string(__wrap_lstat, path, TEST_DIR_DEV "/" TEST_DISK);
    will_return(__wrap_lstat, 0);

    expect_string(__wrap_open, path, TEST_DIR_DEV "/" TEST_DISK);
    expect_any(__wrap_open, flags);
    will_return(__wrap_open, cominitBlockDeviceFd);

    expect_value(__wrap_ioctl, fd, cominitBlockDeviceFd);

    expect_value(__wrap_pread, fd, cominitBlockDeviceFd);
    will_return(__wrap_pread, sizeof(cominitGPTHeader_t));

    expect_value(__wrap_close, fd, cominitBlockDeviceFd);
    will_return(__wrap_close, 0);

    expect_string(__wrap_open, path, TEST_DIR_DEV "/" TEST_DISK);
    expect_any(__wrap_open, flags);
    will_return(__wrap_open, cominitDiskFd);

    expect_value(__wrap_ioctl, fd, cominitDiskFd);

    expect_value(__wrap_pread, fd, cominitDiskFd);
    will_return(__wrap_pread, GPT_HEADER_DEFAULT_ENTRY_SIZE);

    expect_string(__wrap_strcasecmp, s2, guidType);
    expect_any(__wrap_strcasecmp, s1);
    will_return(__wrap_strcasecmp, 0);

    expect_value(__wrap_close, fd, cominitDiskFd);
    will_return(__wrap_close, 0);

    expect_value(__wrap_closedir, dirp, fakeDirPtr);

    cominitMockCloseEnabled = true;
    cominitMockOpenEnabled = true;
    cominitMockReaddirEnabled = true;
    cominitMockClosedirEnabled = true;
    cominitMockPreadEnabled = true;
    cominitMockStrcasecmpEnabled = true;
    cominitMockOpendirEnabled = true;
    cominitMockLstatEnabled = true;
    assert_int_equal(cominitAutomountFindPartition(&emptyDisk, guidType, partition, partitionSize), EXIT_SUCCESS);
    cominitMockCloseEnabled = false;
    cominitMockOpenEnabled = false;
    cominitMockReaddirEnabled = false;
    cominitMockClosedirEnabled = false;
    cominitMockPreadEnabled = false;
    cominitMockStrcasecmpEnabled = false;
    cominitMockOpendirEnabled = false;
    cominitMockLstatEnabled = false;
}
