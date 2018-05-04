#ifndef UNICORN_ASSERTION_H
#define UNICORN_ASSERTION_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#include "unicorn/types.h"


void unicorn_fail_test(UnicornTest *test, char *format, ...);
bool unicorn_check_assertion(UnicornTest *test, bool assertion_success, char *assertion, char *assertion_file, size_t assertion_line);


#define _UNICORN_TEST_FAILURE(...) \
    ({ \
        unicorn_fail_test(_unicorn_test, __VA_ARGS__); \
        return; \
    })


#define FAIL(...) \
    if (({ unicorn_pipe_assertion_failure(_unicorn_test->result, NULL, __FILE__, __LINE__); true; })) \
    _UNICORN_TEST_FAILURE("" __VA_ARGS__)


#define ASSERT(assertion, ...) \
    if (unicorn_check_assertion(_unicorn_test, (assertion), #assertion, __FILE__, __LINE__)) \
    _UNICORN_TEST_FAILURE("" __VA_ARGS__)


#define _UNICORN_CHECK_EQUAL(left, right) _Generic((left), \
    char *: _Generic((right), \
        char *: strcmp((char *)(uintptr_t)(left), (char *)(uintptr_t)(right)) == 0, \
        default: false), \
    default: (left) == (right))


#define _UNICORN_PRINT_FORMAT(value) _Generic((value), \
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


#define ASSERT_EQ(left, right) do \
    { \
        __typeof__(left + 0) _unicorn_assert_left = (left); \
        __typeof__(right + 0) _unicorn_assert_right = (right); \
        if (unicorn_check_assertion(_unicorn_test, _UNICORN_CHECK_EQUAL(_unicorn_assert_left, _unicorn_assert_right), #left " == " #right, __FILE__, __LINE__)) \
        { \
            char _unicorn_assert_message[1024]; \
            snprintf(_unicorn_assert_message, sizeof (_unicorn_assert_message), "Result is equal to %s instead of %s.", _UNICORN_PRINT_FORMAT(_unicorn_assert_left), _UNICORN_PRINT_FORMAT(_unicorn_assert_right)); \
            _UNICORN_TEST_FAILURE(_unicorn_assert_message, _unicorn_assert_left, _unicorn_assert_right); \
        } \
     } while (0)


#endif
