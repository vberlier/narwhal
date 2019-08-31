#ifndef NARWHAL_ASSERTION_H
#define NARWHAL_ASSERTION_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "narwhal/types.h"

void narwhal_fail_test(NarwhalTest *test, char *format, ...);
bool narwhal_check_assertion(NarwhalTest *test,
                             bool assertion_success,
                             char *assertion,
                             char *assertion_file,
                             size_t assertion_line);

bool narwhal_check_string_equal(char *actual, char *expected);
bool narwhal_check_substring(char *string, char *substring);
bool narwhal_check_memory_equal(const void *actual, const void *expected, size_t size);

char *narwhal_assertion_process_string(char *string);

#define _NARWHAL_TEST_FAILURE(...)                             \
    do                                                         \
    {                                                          \
        narwhal_fail_test(_narwhal_current_test, __VA_ARGS__); \
        return;                                                \
    } while (0)

#define FAIL(...)                                                                            \
    do                                                                                       \
    {                                                                                        \
        if (narwhal_check_assertion(_narwhal_current_test, false, NULL, __FILE__, __LINE__)) \
        {                                                                                    \
            _NARWHAL_TEST_FAILURE("" __VA_ARGS__);                                           \
        }                                                                                    \
    } while (0)

#define ASSERT(assertion, ...)                                                       \
    do                                                                               \
    {                                                                                \
        if (narwhal_check_assertion(                                                 \
                _narwhal_current_test, (assertion), #assertion, __FILE__, __LINE__)) \
        {                                                                            \
            _NARWHAL_TEST_FAILURE("" __VA_ARGS__);                                   \
        }                                                                            \
    } while (0)

#define _NARWHAL_PRINT_FORMAT(value) \
    _Generic((value), \
    char: "%c", \
    signed char: "%hhd", \
    unsigned char: "%hhu", \
    signed short: "%hd", \
    unsigned short: "%hu", \
    signed int: "%d", \
    unsigned int: "%u", \
    long int: "%ld", \
    unsigned long int: "%lu", \
    long long int: "%lld", \
    unsigned long long int: "%llu", \
    float: "%f", \
    double: "%f", \
    long double: "%Lf", \
    char *: "\"%s\"", \
    default: "%p")

#define _NARWHAL_PROCESS_VALUE(value) \
    _Generic((value), \
    char *: narwhal_assertion_process_string((char *)(uintptr_t)(value)), \
    default: (value))

#define _NARWHAL_BINARY_ASSERTION(left, right, check, assertion, message)               \
    do                                                                                  \
    {                                                                                   \
        __typeof__(left + 0) _narwhal_assert_left = (left);                             \
        __typeof__(right + 0) _narwhal_assert_right = (right);                          \
        if (narwhal_check_assertion(_narwhal_current_test,                              \
                                    check(_narwhal_assert_left, _narwhal_assert_right), \
                                    assertion,                                          \
                                    __FILE__,                                           \
                                    __LINE__))                                          \
        {                                                                               \
            char _narwhal_assert_message[1024];                                         \
            snprintf(_narwhal_assert_message,                                           \
                     sizeof(_narwhal_assert_message),                                   \
                     message,                                                           \
                     _NARWHAL_PRINT_FORMAT(_narwhal_assert_left),                       \
                     _NARWHAL_PRINT_FORMAT(_narwhal_assert_right));                     \
            _NARWHAL_TEST_FAILURE(_narwhal_assert_message,                              \
                                  _NARWHAL_PROCESS_VALUE(_narwhal_assert_left),         \
                                  _NARWHAL_PROCESS_VALUE(_narwhal_assert_right));       \
        }                                                                               \
    } while (0)

#define _NARWHAL_CHECK_EQ(left, right) \
    _Generic((left), \
    char *: _Generic((right), \
        char *: narwhal_check_string_equal((char *)(uintptr_t)(left), (char *)(uintptr_t)(right)), \
        default: false), \
    default: (left) == (right))

#define _NARWHAL_CHECK_NE(left, right) \
    _Generic((left), \
    char *: _Generic((right), \
        char *: (!narwhal_check_string_equal((char *)(uintptr_t)(left), (char *)(uintptr_t)(right))), \
        default: true), \
    default: (left) != (right))

#define _NARWHAL_CHECK_LT(left, right) (left) < (right)

#define _NARWHAL_CHECK_LE(left, right) (left) <= (right)

#define _NARWHAL_CHECK_GT(left, right) (left) > (right)

#define _NARWHAL_CHECK_GE(left, right) (left) >= (right)

#define _NARWHAL_CHECK_SUBSTRING(left, right) narwhal_check_substring(left, right)

#define _NARWHAL_CHECK_NOT_SUBSTRING(left, right) (!narwhal_check_substring(left, right))

#define ASSERT_EQ(left, right)                     \
    _NARWHAL_BINARY_ASSERTION(left,                \
                              right,               \
                              _NARWHAL_CHECK_EQ,   \
                              #left " == " #right, \
                              "First argument %s is not equal to %s.")

#define ASSERT_NE(left, right)                     \
    _NARWHAL_BINARY_ASSERTION(left,                \
                              right,               \
                              _NARWHAL_CHECK_NE,   \
                              #left " != " #right, \
                              "First argument %s is not different from %s.")

#define ASSERT_LT(left, right)                    \
    _NARWHAL_BINARY_ASSERTION(left,               \
                              right,              \
                              _NARWHAL_CHECK_LT,  \
                              #left " < " #right, \
                              "First argument %s is not less than %s.")

#define ASSERT_LE(left, right)                     \
    _NARWHAL_BINARY_ASSERTION(left,                \
                              right,               \
                              _NARWHAL_CHECK_LE,   \
                              #left " <= " #right, \
                              "First argument %s is not less than or equal to %s.")

#define ASSERT_GT(left, right)                    \
    _NARWHAL_BINARY_ASSERTION(left,               \
                              right,              \
                              _NARWHAL_CHECK_GT,  \
                              #left " > " #right, \
                              "First argument %s is not greater than %s.")

#define ASSERT_GE(left, right)                     \
    _NARWHAL_BINARY_ASSERTION(left,                \
                              right,               \
                              _NARWHAL_CHECK_GE,   \
                              #left " >= " #right, \
                              "First argument %s is not greater or equal to %s.")

#define ASSERT_SUBSTRING(string, substring)                                  \
    _NARWHAL_BINARY_ASSERTION(string,                                        \
                              substring,                                     \
                              _NARWHAL_CHECK_SUBSTRING,                      \
                              "strstr(" #string ", " #substring ") != NULL", \
                              "First argument %s doesn't contain %s.")

#define ASSERT_NOT_SUBSTRING(string, substring)                              \
    _NARWHAL_BINARY_ASSERTION(string,                                        \
                              substring,                                     \
                              _NARWHAL_CHECK_NOT_SUBSTRING,                  \
                              "strstr(" #string ", " #substring ") == NULL", \
                              "First argument %s contains %s.")

#define ASSERT_MEMORY(left, right, size)                                                 \
    do                                                                                   \
    {                                                                                    \
        if (narwhal_check_assertion(_narwhal_current_test,                               \
                                    narwhal_check_memory_equal((left), (right), (size)), \
                                    "memcmp(" #left ", " #right ", " #size ") == 0",     \
                                    __FILE__,                                            \
                                    __LINE__))                                           \
        {                                                                                \
            _NARWHAL_TEST_FAILURE("");                                                   \
        }                                                                                \
    } while (0)

#endif
