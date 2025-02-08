#pragma once
#ifndef __ILMARTO_TEST_H_
#define __ILMARTO_TEST_H_

#include <stdio.h>

#define __TT_PASSED_COLOR "\e[1;32m"
#define __TT_FAILED_COLOR "\e[1;31m"
#define __TT_WARN_COLOR "\e[1;33m"
#define __TT_CLEAR_COLOR "\e[0m"
#define __TT_PERCENT_COLOR(percent) (percent < 50 ? __TT_FAILED_COLOR : percent < 75 ? __TT_WARN_COLOR \
                                                                                     : __TT_PASSED_COLOR)

#define __TT_ASSERT_PASSED(statement) __TT_PASSED_COLOR "[O]"__TT_CLEAR_COLOR \
                                                        " %s: \"" #statement "\""
#define __TT_ASSERT_FAILED(statement) __TT_FAILED_COLOR "[X]"__TT_CLEAR_COLOR \
                                                        " %s: \"" #statement "\""

#define __TT_GROUP_RESULT "|\n+~ Tests performed: %llu\n+~ Tests passed:    %s%lld%% (%lld)"__TT_CLEAR_COLOR

// Test count struct
typedef struct __Test_Count {
    long long unsigned passed;
    long long unsigned failed;
} __Test_Count;

/**
 * @brief Defines a test group.
 * @param name Name of the test group.
 * @param testGroupBlock Test group code block.
 */
#define TestGroup(name, testGroupBlock)                                                                 \
    do {                                                                                                \
        __Test_Count testGroup = { 0 };                                                                 \
        printf("%s\n", name);                                                                           \
        {                                                                                               \
            testGroupBlock                                                                              \
        }                                                                                               \
        long long unsigned total = testGroup.passed + testGroup.failed;                                 \
        long long unsigned percentage = testGroup.passed * 100 / total;                                 \
        printf(__TT_GROUP_RESULT, total, __TT_PERCENT_COLOR(percentage), percentage, testGroup.passed); \
    } while (0)

/**
 * @brief Defines a test case.
 * @param name Name of the test case.
 * @param testCaseBlock Test case code block.
 */
#define TestCase(name, testCaseBlock)     \
    do {                                  \
        printf("|\n+> %s\n|  |\n", name); \
        {                                 \
            testCaseBlock                 \
        }                                 \
    } while (0)

/**
 * @brief Assets an statement.
 * @param msg Message of the assert.
 * @param statement Statement to assert.
 */
#define Assert(msg, statement)                                        \
    do {                                                              \
        if (statement) {                                              \
            printf("|  +> " __TT_ASSERT_PASSED(statement) "\n", msg); \
            ++testGroup.passed;                                       \
        } else {                                                      \
            printf("|  +> " __TT_ASSERT_FAILED(statement) "\n", msg); \
            ++testGroup.failed;                                       \
        }                                                             \
    } while (0)

/**
 * @brief Assets an statement to be `true`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertTrue(msg, value) Assert(msg, (0 == !(value)))
/**
 * @brief Assets an statement to be `false`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertFalse(msg, value) Assert(msg, (1 == !(value)))

/**
 * @brief Assets an statement to be `NULL`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertNull(msg, value) Assert(msg, (NULL == (value)))
/**
 * @brief Assets an statement to not be `NULL`.
 * @param msg Message of the assert.
 * @param value Value to check.
 */
#define AssertNotNull(msg, value) Assert(msg, (NULL != (value)))

// #ifdef USE_STRING_ASSERTS

//     #include <string.h>

//     strncmp()

//     #define AssertSameString(message, value) Assert(message, ())
//     #define AssertDiferentString(message, value) Assert(message, ())

// #endif // USE_STRING_ASSERTS

#endif // __ILMARTO_TEST_H_
