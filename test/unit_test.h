// SPDX-License-Identifier: MIT
/**
 * @file unit_test.h
 * @brief Header for common definitions/includes specifically for cmocka unit tests.
 */
#ifndef __UNIT_TEST_H__
#define __UNIT_TEST_H__

// clang-format off
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <cmocka.h>
// clang-format on
//     Rationale: Specific order of includes needed by cmocka.h.

/* should be moved to cmocka extension if available */
static inline void assert_memory_is_zeroed(const void *bufferToVerify, size_t bufferSize) {
    const uint8_t *buffer = (const uint8_t *)bufferToVerify;
    for (size_t index = 0; index < bufferSize; index++) {
        assert_int_equal(buffer[index], 0);
    }
}

#endif /* __UNIT_TEST_H__ */
