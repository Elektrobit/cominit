// SPDX-License-Identifier: MIT
/**
 * @file utest-automount-find-partition-on-disk.c
 * @brief Implementation of an cominitAutomountFindPartitionOnDisk() unit test group using cmocka.
 */
#include "utest-automount-find-partition-on-disk.h"

#include "common.h"
#include "unit_test.h"

#if defined(__GLIBC__)
#define IOCTL_REQ_T unsigned long
#else
#define IOCTL_REQ_T int
#endif

int cominitDiskFd = 42;
int cominitDiskFdFailure = 4711;

bool cominitMockPreadEnabled = false;
// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
ssize_t __wrap_pread(int fd, void *buf, size_t count, off_t offset) {
    if (cominitMockPreadEnabled) {
        COMINIT_PARAM_UNUSED(offset);
        assert_non_null(buf);
        check_expected(fd);
        if (fd == cominitDiskFd) {
            /*set entry to non zero*/
            memset(buf, 1, GPT_HEADER_DEFAULT_ENTRY_SIZE);
            return count;
        }
        if (fd == cominitDiskFdFailure) {
            return count;
        }
        return mock_type(ssize_t);
    } else {
        return __real_pread(fd, buf, count, offset);
    }
}

// NOLINTNEXTLINE(readability-identifier-naming)    Rationale: Naming scheme fixed due to linker wrapping.
int __wrap_ioctl(int fd, unsigned long request, ...) {
    check_expected(fd);

    va_list ap;
    va_start(ap, request);

    switch (request) {
        case BLKSSZGET: {
            int *out = va_arg(ap, int *);
            assert_non_null(out);
            va_end(ap);
            return 0;
        }
        case BLKGETSIZE64: {
            uint64_t *out = va_arg(ap, uint64_t *);
            assert_non_null(out);
            *out = 1024 * 1024;
            va_end(ap);
            return 0;
        }
        default:
            va_end(ap);
            return -1;
    }
}

/**
 * Run the unit tests for cominitAutomountFindPartitionOnDisk().
 *
 * @return  The same as cmocka_run_group_tests() returns for the tests.
 */
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(cominitAutomountFindPartitionOnDiskTestSuccess),
        cmocka_unit_test(cominitAutomountFindPartitionOnDiskTestParamFailure),
        cmocka_unit_test(cominitAutomountFindPartitionOnDiskTestGptIsNegativeFailure),
        cmocka_unit_test(cominitAutomountFindPartitionOnDiskTestGptIsZeroFailure),
        cmocka_unit_test(cominitAutomountFindPartitionOnDiskTestGptEntrySizeIsNegativeFailure),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
