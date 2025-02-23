#pragma once
#ifndef __ILMARTO_TEST_H_
#define __ILMARTO_TEST_H_

#include <stdio.h>
#include <string.h>

// ---- Intrinsics ----------------------------------------------------------------------
#define __ILMARTO_TEST_PASSED_COLOR "\e[1;32m"
#define __ILMARTO_TEST_FAILED_COLOR "\e[1;31m"
#define __ILMARTO_TEST_WARN_COLOR "\e[1;33m"
#define __ILMARTO_TEST_INFO_COLOR "\e[0;36m"
#define __ILMARTO_TEST_CLEAR_COLOR "\e[0m"
#define __ILMARTO_TEST_PERCENT_COLOR(percent)            \
    (percent < 50          ? __ILMARTO_TEST_FAILED_COLOR \
            : percent < 75 ? __ILMARTO_TEST_WARN_COLOR   \
                           : __ILMARTO_TEST_PASSED_COLOR)

#define __ILMARTO_TEST_ASSERT_PASSED(display) \
    __ILMARTO_TEST_PASSED_COLOR               \
    "[O]"__ILMARTO_TEST_CLEAR_COLOR           \
    " %s: " __ILMARTO_TEST_INFO_COLOR display __ILMARTO_TEST_CLEAR_COLOR
#define __ILMARTO_TEST_ASSERT_FAILED(display) \
    __ILMARTO_TEST_FAILED_COLOR               \
    "[X]"__ILMARTO_TEST_CLEAR_COLOR           \
    " %s: " __ILMARTO_TEST_INFO_COLOR display __ILMARTO_TEST_CLEAR_COLOR

#define __ILMARTO_TEST_GROUP_RESULT \
    "|\n+~ Tests performed: %llu\n+~ Tests passed:    %s%lld%% (%lld)"__ILMARTO_TEST_CLEAR_COLOR
#define __ILMARTO_TEST_TEST_GROUP(name, testGroupBlock)                                 \
    do {                                                                                \
        struct {                                                                        \
            long long unsigned passed;                                                  \
            long long unsigned failed;                                                  \
        } testGroup = { 0 };                                                            \
        printf("\n%s\n", name);                                                         \
        {                                                                               \
            testGroupBlock                                                              \
        }                                                                               \
        long long unsigned total = testGroup.passed + testGroup.failed;                 \
        long long unsigned percentage = total > 0 ? testGroup.passed * 100 / total : 0; \
        printf(__ILMARTO_TEST_GROUP_RESULT "\n", total,                                 \
            __ILMARTO_TEST_PERCENT_COLOR(percentage), percentage,                       \
            testGroup.passed);                                                          \
    } while (0)
#define __ILMARTO_TEST_TEST_CASE(name, testCaseBlock) \
    do {                                              \
        printf("|\n+> %s\n|  |\n", name);             \
        {                                             \
            testCaseBlock                             \
        }                                             \
    } while (0)
#define __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, statement, display)            \
    do {                                                                      \
        if (statement) {                                                      \
            printf("|  +> " __ILMARTO_TEST_ASSERT_PASSED(display) "\n", msg); \
            ++testGroup.passed;                                               \
        } else {                                                              \
            printf("|  +> " __ILMARTO_TEST_ASSERT_FAILED(display) "\n", msg); \
            ++testGroup.failed;                                               \
        }                                                                     \
    } while (0)
#define __ILMARTO_TEST_ASSERT(msg, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, value, "assert(" #value ")")
#define __ILMARTO_TEST_ASSERT_TRUE(msg, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, (0 == !(value)), "assert_true(" #value ")")
#define __ILMARTO_TEST_ASSERT_FALSE(msg, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, (1 == !(value)), "assert_false(" #value ")")
#define __ILMARTO_TEST_ASSERT_NULL(msg, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, (NULL == (value)), "assert_null(" #value ")")
#define __ILMARTO_TEST_ASSERT_NOT_NULL(msg, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, (NULL != (value)), "assert_not_null(" #value ")")
#define __ILMARTO_TEST_ASSERT_MEM_CONTENTS(msg, expected, value, mem_size) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, 0 == memncmp((char*)expected, (char*)value, mem_size), "assert_mem_same(" #expected ", " #value ")")
#define __ILMARTO_TEST_ASSERT_MEM_ZERO(msg, value, mem_size) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, 1 == __ILMARTO_TEST_IS_MEMORY_ZERO(value, mem_size), "assert_mem_zero(" #value ")")
#define __ILMARTO_TEST_ASSERT_SAME_STRING(msg, expected, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, 0 == strcmp(expected, value), "assert_string_same(\"" #expected "\", \"" #value "\")")
#define __ILMARTO_TEST_ASSERT_DIFFERENT_STRING(msg, expected, value) __ILMARTO_TEST_ASSERT_AND_DISPLAY(msg, 0 == strcmp(expected, value), "assert_string_different(\"" #expected "\", \"" #value "\")")

static inline int __ILMARTO_TEST_IS_MEMORY_ZERO(const void* memory, size_t n)
{
    for (size_t i = 0; i < n; i++) {
        if (((char*)memory)[i] != 0) { return 0; }
    }
    return 1;
}
// --------------------------------------------------------------------------------------

/**
 * Defines a test group.
 * @param name Name of the test group.
 * @param testGroupBlock Test group code block.
 */
#define TestGroup(name, testGroupBlock) __ILMARTO_TEST_TEST_GROUP(name, testGroupBlock)

/**
 * Defines a test case.
 * @param name Name of the test case.
 * @param testCaseBlock Test case code block.
 */
#define TestCase(name, testCaseBlock) __ILMARTO_TEST_TEST_CASE(name, testCaseBlock)

/**
 * Assets an statement.
 * @param msg Message of the assert.
 * @param statement Statement to assert.
 */
#define Assert(msg, statement) __ILMARTO_TEST_ASSERT(msg, statement)

/**
 * Assets an statement to be `true`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertTrue(msg, value) __ILMARTO_TEST_ASSERT_TRUE(msg, value)
/**
 * Assets an statement to be `false`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertFalse(msg, value) __ILMARTO_TEST_ASSERT_FALSE(msg, value)

/**
 * Assets an statement to be `NULL`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertNull(msg, value) __ILMARTO_TEST_ASSERT_NULL(msg, value)
/**
 * Assets an statement to not be `NULL`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertNotNull(msg, value) __ILMARTO_TEST_ASSERT_NOT_NULL(msg, value)

/**
 * Asserts the contents of two memory spaces to be the same.
 * @param msg Message of the assert.
 * @param expected Expected memory contents.
 * @param value Memory contents.
 * @param mem_size Memory bytes to check.
 */
#define AssertMemContents(msg, expected, value, mem_size) __ILMARTO_TEST_ASSERT_MEM_CONTENTS(msg, expected, value, mem_size)
/**
 * Asserts the contents a memory space to be zero.
 * @param msg Message of the assert.
 * @param value Memory contents.
 * @param mem_size Memory bytes to check.
 */
#define AssertMemZero(msg, value, mem_size) __ILMARTO_TEST_ASSERT_MEM_ZERO(msg, value, mem_size)

/**
 * Asserts if two null-teminated strings are the same.
 * @param msg Message of the assert.
 * @param expected Expected string.
 * @param value String to check.
 */
#define AssertSameString(message, expected, value) __ILMARTO_TEST_ASSERT_SAME_STRING(message, expected, value)
/**
 * Asserts if two null-teminated strings are different.
 * @param msg Message of the assert.
 * @param expected Expected string.
 * @param value String to check.
 */
#define AssertDifferentString(message, expected, value) __ILMARTO_TEST_ASSERT_DIFFERENT_STRING(message, expected, value)

#endif // __ILMARTO_TEST_H_
