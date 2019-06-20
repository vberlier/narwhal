/*
Unicorn v0.1.0 (https://github.com/vberlier/unicorn)
Amalgamated header file

Generated with amalgamate.py (https://github.com/edlund/amalgamate)

MIT License

Copyright (c) 2019 Valentin Berlier

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef UNICORN_H
#define UNICORN_H


// #include "unicorn/types.h"
#ifndef UNICORN_TYPES_H
#define UNICORN_TYPES_H


// #include "unicorn/collection/types.h"
#ifndef UNICORN_COLLECTION_TYPES_H
#define UNICORN_COLLECTION_TYPES_H


typedef struct UnicornCollection UnicornCollection;
typedef struct UnicornCollectionItem UnicornCollectionItem;


#endif

// #include "unicorn/diff/types.h"
#ifndef UNICORN_DIFF_TYPES_H
#define UNICORN_DIFF_TYPES_H


typedef struct UnicornDiffMatrix UnicornDiffMatrix;
typedef enum UnicornDiffChunkType UnicornDiffChunkType;
typedef struct UnicornDiff UnicornDiff;
typedef struct UnicornDiffChunk UnicornDiffChunk;


#endif

// #include "unicorn/fixture/types.h"
#ifndef UNICORN_FIXTURE_TYPES_H
#define UNICORN_FIXTURE_TYPES_H


typedef struct UnicornTestFixture UnicornTestFixture;

typedef void (*UnicornTestFixtureSetup)(void *value, UnicornTestFixture *test_fixture);
typedef void (*UnicornTestFixtureCleanup)(void *value, UnicornTestFixture *test_fixture);


#endif

// #include "unicorn/group/types.h"
#ifndef UNICORN_GROUP_TYPES_H
#define UNICORN_GROUP_TYPES_H


typedef struct UnicornTestGroup UnicornTestGroup;

typedef void (*UnicornGroupItemRegistration)(UnicornTestGroup *test_group);


#endif

// #include "unicorn/param/types.h"
#ifndef UNICORN_PARAM_TYPES_H
#define UNICORN_PARAM_TYPES_H


typedef struct UnicornTestParam UnicornTestParam;


#endif

// #include "unicorn/result/types.h"
#ifndef UNICORN_RESULT_TYPES_H
#define UNICORN_RESULT_TYPES_H


typedef struct UnicornTestResult UnicornTestResult;
typedef struct UnicornTestParamSnapshot UnicornTestParamSnapshot;


#endif

// #include "unicorn/session/types.h"
#ifndef UNICORN_SESSION_TYPES_H
#define UNICORN_SESSION_TYPES_H


typedef struct UnicornTestSession UnicornTestSession;
typedef struct UnicornSessionOutputState UnicornSessionOutputState;


#endif

// #include "unicorn/test/types.h"
#ifndef UNICORN_TEST_TYPES_H
#define UNICORN_TEST_TYPES_H


typedef struct UnicornTest UnicornTest;

typedef void (*UnicornTestModifierRegistration)(UnicornTest *test, UnicornCollection *params, UnicornCollection *fixtures);
typedef void (*UnicornTestFunction)();


#endif

// #include "unicorn/test_utils/types.h"
#ifndef UNICORN_TEST_UTILS_TYPES_H
#define UNICORN_TEST_UTILS_TYPES_H


typedef struct UnicornOutputCapture UnicornOutputCapture;


#endif



#endif


// #include "unicorn/assertion/assertion.h"
#ifndef UNICORN_ASSERTION_H
#define UNICORN_ASSERTION_H


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// #include "unicorn/types.h"



void unicorn_fail_test(UnicornTest *test, char *format, ...);
bool unicorn_check_assertion(UnicornTest *test, bool assertion_success, char *assertion, char *assertion_file, size_t assertion_line);

bool unicorn_check_string_equal(char *actual, char *expected);
bool unicorn_check_substring(char *string, char *substring);

char *unicorn_assertion_process_string(char *string);


#define _UNICORN_TEST_FAILURE(...) \
    ({ \
        unicorn_fail_test(_unicorn_current_test, __VA_ARGS__); \
        return; \
    })


#define FAIL(...) \
    if (({ unicorn_pipe_assertion_failure(_unicorn_current_test->result, NULL, __FILE__, __LINE__); true; })) \
    _UNICORN_TEST_FAILURE("" __VA_ARGS__)


#define ASSERT(assertion, ...) \
    if (unicorn_check_assertion(_unicorn_current_test, (assertion), #assertion, __FILE__, __LINE__)) \
    _UNICORN_TEST_FAILURE("" __VA_ARGS__)


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


#define _UNICORN_PROCESS_VALUE(value) _Generic((value), \
    char *: unicorn_assertion_process_string((char *)(uintptr_t)(value)), \
    default: (value))


#define _UNICORN_BINARY_ASSERTION(left, right, check, assertion, message) do \
    { \
        __typeof__(left + 0) _unicorn_assert_left = (left); \
        __typeof__(right + 0) _unicorn_assert_right = (right); \
        if (unicorn_check_assertion(_unicorn_current_test, check(_unicorn_assert_left, _unicorn_assert_right), assertion, __FILE__, __LINE__)) \
        { \
            char _unicorn_assert_message[1024]; \
            snprintf(_unicorn_assert_message, sizeof (_unicorn_assert_message), message, _UNICORN_PRINT_FORMAT(_unicorn_assert_left), _UNICORN_PRINT_FORMAT(_unicorn_assert_right)); \
            _UNICORN_TEST_FAILURE(_unicorn_assert_message, _UNICORN_PROCESS_VALUE(_unicorn_assert_left), _UNICORN_PROCESS_VALUE(_unicorn_assert_right)); \
        } \
     } while (0)


#define _UNICORN_CHECK_EQ(left, right) _Generic((left), \
    char *: _Generic((right), \
        char *: unicorn_check_string_equal((char *)(uintptr_t)(left), (char *)(uintptr_t)(right)), \
        default: false), \
    default: (left) == (right))

#define _UNICORN_CHECK_NE(left, right) _Generic((left), \
    char *: _Generic((right), \
        char *: (!unicorn_check_string_equal((char *)(uintptr_t)(left), (char *)(uintptr_t)(right))), \
        default: true), \
    default: (left) != (right))

#define _UNICORN_CHECK_LT(left, right) (left) < (right)

#define _UNICORN_CHECK_LE(left, right) (left) <= (right)

#define _UNICORN_CHECK_GT(left, right) (left) > (right)

#define _UNICORN_CHECK_GE(left, right) (left) >= (right)

#define _UNICORN_CHECK_SUBSTRING(left, right) unicorn_check_substring(left, right)

#define _UNICORN_CHECK_NOT_SUBSTRING(left, right) (!unicorn_check_substring(left, right))


#define ASSERT_EQ(left, right) \
    _UNICORN_BINARY_ASSERTION(left, right, _UNICORN_CHECK_EQ, #left " == " #right, "First argument %s is not equal to %s.")

#define ASSERT_NE(left, right) \
    _UNICORN_BINARY_ASSERTION(left, right, _UNICORN_CHECK_NE, #left " != " #right, "First argument %s is not different from %s.")

#define ASSERT_LT(left, right) \
    _UNICORN_BINARY_ASSERTION(left, right, _UNICORN_CHECK_LT, #left " < " #right, "First argument %s is not less than %s.")

#define ASSERT_LE(left, right) \
    _UNICORN_BINARY_ASSERTION(left, right, _UNICORN_CHECK_LE, #left " <= " #right, "First argument %s is not less than or equal to %s.")

#define ASSERT_GT(left, right) \
    _UNICORN_BINARY_ASSERTION(left, right, _UNICORN_CHECK_GT, #left " > " #right, "First argument %s is not greater than %s.")

#define ASSERT_GE(left, right) \
    _UNICORN_BINARY_ASSERTION(left, right, _UNICORN_CHECK_GE, #left " >= " #right, "First argument %s is not greater or equal to %s.")

#define ASSERT_SUBSTRING(string, substring) \
    _UNICORN_BINARY_ASSERTION(string, substring, _UNICORN_CHECK_SUBSTRING, "strstr(" #string ", " #substring ") != NULL", "First argument %s doesn't contain %s.")

#define ASSERT_NOT_SUBSTRING(string, substring) \
    _UNICORN_BINARY_ASSERTION(string, substring, _UNICORN_CHECK_NOT_SUBSTRING, "strstr(" #string ", " #substring ") == NULL", "First argument %s contains %s.")


#endif

// #include "unicorn/collection/collection.h"
#ifndef UNICORN_COLLECTION_H
#define UNICORN_COLLECTION_H


#include <stdlib.h>
#include <stdbool.h>

// #include "unicorn/types.h"



struct UnicornCollection
{
    size_t count;
    UnicornCollectionItem *first;
    UnicornCollectionItem *last;
};


struct UnicornCollectionItem
{
    void *value;
    UnicornCollectionItem *previous;
    UnicornCollectionItem *next;
};


UnicornCollection *unicorn_empty_collection();
void unicorn_collection_append(UnicornCollection *collection, void *value);
void *unicorn_collection_pop(UnicornCollection *collection);
void unicorn_free_collection(UnicornCollection *collection);


// Foreach macros

#define UNICORN_EACH(item_variable, collection) \
    for ( \
        UnicornCollectionItem *_unicorn_loop_item = (collection)->first; \
        _unicorn_loop_item != NULL && ((item_variable) = _unicorn_loop_item->value, true); \
        _unicorn_loop_item = _unicorn_loop_item->next \
    )

#define UNICORN_REVERSED(item_variable, collection) \
    for ( \
        UnicornCollectionItem *_unicorn_loop_item = (collection)->last; \
        _unicorn_loop_item != NULL && ((item_variable) = _unicorn_loop_item->value, true); \
        _unicorn_loop_item = _unicorn_loop_item->previous \
    )

#endif

// #include "unicorn/diff/diff.h"
#ifndef UNICORN_DIFF_H
#define UNICORN_DIFF_H


// #include "unicorn/types.h"



struct UnicornDiffMatrix
{
    size_t rows;
    size_t columns;
    int *content;
};

enum UnicornDiffChunkType
{
    UNICORN_DIFF_CHUNK_TYPE_MATCHED,
    UNICORN_DIFF_CHUNK_TYPE_ADDED,
    UNICORN_DIFF_CHUNK_TYPE_REPLACED,
    UNICORN_DIFF_CHUNK_TYPE_DELETED
};

struct UnicornDiff
{
    size_t size;
    UnicornDiffChunk *chunks;
};

struct UnicornDiffChunk
{
    UnicornDiffChunkType type;
    size_t original_start;
    size_t original_end;
    size_t modified_start;
    size_t modified_end;
};

UnicornDiffMatrix *unicorn_new_diff_matrix(size_t rows, size_t columns);
UnicornDiffMatrix *unicorn_new_diff_matrix_from_lengths(size_t original_length, size_t modified_lengths);
void unicorn_diff_matrix_fill_from_strings(UnicornDiffMatrix *diff_matrix, char *original, char *modified);
void unicorn_diff_matrix_fill_from_lines(UnicornDiffMatrix *diff_matrix, char *original, char *modified);
UnicornDiff unicorn_diff_matrix_get_diff(UnicornDiffMatrix *diff_matrix);

size_t unicorn_diff_matrix_index(UnicornDiffMatrix *diff_matrix, size_t row, size_t column);
int unicorn_diff_matrix_get(UnicornDiffMatrix *diff_matrix, size_t row, size_t column);
void unicorn_diff_matrix_set(UnicornDiffMatrix *diff_matrix, size_t row, size_t column, int value);

UnicornDiff unicorn_diff_strings_lengths(char *original, size_t original_length, char *modified, size_t modified_length);
UnicornDiff unicorn_diff_strings(char *original, char *modified);
UnicornDiff unicorn_diff_lines(char *original, char *modified);

void unicorn_free_diff_matrix(UnicornDiffMatrix *diff_matrix);


#endif

// #include "unicorn/fixture/fixture.h"
#ifndef UNICORN_FIXTURE_H
#define UNICORN_FIXTURE_H


#include <stdlib.h>

// #include "unicorn/types.h"



extern UnicornCollection *_unicorn_current_fixtures;


struct UnicornTestFixture
{
    char *name;
    size_t size;
    void *value;
    UnicornTestFixtureSetup setup;
    UnicornTestFixtureCleanup cleanup;
    UnicornTest *test;
    UnicornCollection *accessible_fixtures;
    UnicornCollection *accessible_params;
};

UnicornTestFixture *unicorn_new_test_fixture(char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTest *test, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);
UnicornTestFixture *unicorn_get_test_fixture(UnicornCollection *fixtures, char *fixture_name);
void unicorn_free_test_fixture(UnicornTestFixture *test_fixture);


#define DECLARE_FIXTURE(fixture_name, fixture_type) \
    typedef fixture_type _unicorn_fixture_type_ ## fixture_name; \
    extern UnicornTestModifierRegistration _unicorn_test_fixture_modifiers_ ## fixture_name[]; \
    void _unicorn_fixture_ ## fixture_name ## _setup(_unicorn_fixture_type_ ## fixture_name *fixture_name, UnicornTestFixture *_unicorn_test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _call_setup(void *value, UnicornTestFixture *test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _cleanup(_unicorn_fixture_type_ ## fixture_name *fixture_name, UnicornTestFixture *_unicorn_test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _call_cleanup(void *value, UnicornTestFixture *test_fixture); \
    void fixture_name(UnicornTest *test, UnicornCollection *params, UnicornCollection *fixtures) \


#define TEST_FIXTURE(fixture_name, fixture_type, ...) \
    DECLARE_FIXTURE(fixture_name, fixture_type); \
    UnicornTestModifierRegistration _unicorn_test_fixture_modifiers_ ## fixture_name[] = { __VA_ARGS__ }; \
    void _unicorn_fixture_ ## fixture_name ## _call_setup(void *value, UnicornTestFixture *test_fixture) \
    { \
        _unicorn_fixture_ ## fixture_name ## _setup((_unicorn_fixture_type_ ## fixture_name *)value, test_fixture); \
    } \
    void fixture_name(UnicornTest *test, UNUSED UnicornCollection *params, UnicornCollection *fixtures) \
    { \
        unicorn_register_test_fixture(test, fixtures, #fixture_name, sizeof (_unicorn_fixture_type_ ## fixture_name), _unicorn_fixture_ ## fixture_name ## _call_setup, _unicorn_test_fixture_modifiers_ ## fixture_name, sizeof (_unicorn_test_fixture_modifiers_ ## fixture_name) / sizeof (*_unicorn_test_fixture_modifiers_ ## fixture_name)); \
    } \
    void _unicorn_fixture_ ## fixture_name ## _setup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture)


#define GET_FIXTURE(fixture_name) \
    _unicorn_fixture_type_ ## fixture_name fixture_name = \
    ({ \
        UnicornTestFixture *_unicorn_test_fixture_ ## fixture_name = unicorn_get_test_fixture(_unicorn_current_fixtures, #fixture_name); \
        if (_unicorn_test_fixture_ ## fixture_name == NULL) FAIL("Fixture \"%s\" hasn't been applied to the current context.", #fixture_name); \
        *(_unicorn_fixture_type_ ## fixture_name *)_unicorn_test_fixture_ ## fixture_name->value; \
    })


#define CLEANUP_FIXTURE(fixture_name) \
        _unicorn_test_fixture->cleanup = _unicorn_fixture_ ## fixture_name ## _call_cleanup; \
    } \
    void _unicorn_fixture_ ## fixture_name ## _call_cleanup(void *value, UnicornTestFixture *test_fixture) \
    { \
        _unicorn_fixture_ ## fixture_name ## _cleanup((_unicorn_fixture_type_ ## fixture_name *)value, test_fixture); \
    } \
    void _unicorn_fixture_ ## fixture_name ## _cleanup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture) \
    {


#endif

// #include "unicorn/group/group.h"
#ifndef UNICORN_GROUP_H
#define UNICORN_GROUP_H


#include <stdlib.h>

// #include "unicorn/types.h"

// #include "unicorn/unused_attribute.h"
#ifndef UNICORN_UNUSED_ATTRIBUTE_H
#define UNICORN_UNUSED_ATTRIBUTE_H


#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif


#endif



struct UnicornTestGroup
{
    char *name;
    UnicornTestGroup *group;
    UnicornCollection *subgroups;
    UnicornCollection *tests;
};

UnicornTestGroup *unicorn_new_test_group(char *name, UnicornGroupItemRegistration *group_items, size_t item_count);
void unicorn_register_subgroup(UnicornTestGroup *test_group, char *name, UnicornGroupItemRegistration *group_items, size_t item_count);
void unicorn_register_test(UnicornTestGroup *test_group, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);

void unicorn_free_test_group(UnicornTestGroup *test_group);


#define DECLARE_GROUP(group_name) \
    extern UnicornGroupItemRegistration _unicorn_group_items_ ## group_name[]; \
    void group_name(UnicornTestGroup *test_group)


#define TEST_GROUP(group_name, ...) \
    DECLARE_GROUP(group_name); \
    UnicornGroupItemRegistration _unicorn_group_items_ ## group_name[] = __VA_ARGS__; \
    void group_name(UnicornTestGroup *test_group) \
    { \
        unicorn_register_subgroup(test_group, #group_name, _unicorn_group_items_ ## group_name, sizeof (_unicorn_group_items_ ## group_name) / sizeof (*_unicorn_group_items_ ## group_name)); \
    }


#endif

// #include "unicorn/output/output.h"
#ifndef UNICORN_OUTPUT_H
#define UNICORN_OUTPUT_H


#include <stdio.h>

// #include "unicorn/types.h"



void unicorn_output_string(FILE* stream, char *string, size_t line_number, char *indent);

void unicorn_output_session_init(UnicornTestSession *test_session);
void unicorn_output_session_progress(UnicornTestSession *test_session);
void unicorn_output_session_result(UnicornTestSession *test_session);


#endif

// #include "unicorn/param/param.h"
#ifndef UNICORN_PARAM_H
#define UNICORN_PARAM_H


#include <stdlib.h>

// #include "unicorn/types.h"



extern UnicornCollection *_unicorn_current_params;


struct UnicornTestParam
{
    char *name;
    size_t index;
    size_t count;
    void *values;
    UnicornTest *test;
};

UnicornTestParam *unicorn_new_test_param(char *name, void *values, size_t count, UnicornTest *test);
UnicornTestParam *unicorn_get_test_param(UnicornCollection *params, char *param_name);
void unicorn_free_test_param(UnicornTestParam *test_param);


#define DECLARE_PARAM(param_name, param_type) \
    typedef param_type _unicorn_param_type_ ## param_name; \
    extern _unicorn_param_type_ ## param_name _unicorn_param_ ## param_name[]; \
    void param_name(UnicornTest *test, UnicornCollection *params, UnicornCollection *fixtures)


#define TEST_PARAM(param_name, param_type, ...) \
    DECLARE_PARAM(param_name, param_type); \
    _unicorn_param_type_ ## param_name _unicorn_param_ ## param_name[] = __VA_ARGS__; \
    void param_name(UnicornTest *test, UnicornCollection *params, UNUSED UnicornCollection *fixtures) \
    { \
        unicorn_register_test_param(test, params, #param_name, _unicorn_param_ ## param_name, sizeof (_unicorn_param_ ## param_name) / sizeof (*_unicorn_param_ ## param_name)); \
    }


#define GET_PARAM(param_name) \
    _unicorn_param_type_ ## param_name param_name = \
    ({ \
        UnicornTestParam *_unicorn_test_param_ ## param_name = unicorn_get_test_param(_unicorn_current_params, #param_name); \
        if (_unicorn_test_param_ ## param_name == NULL) FAIL("Parameter \"%s\" hasn't been applied to the current context.", #param_name); \
        ((_unicorn_param_type_ ## param_name *)_unicorn_test_param_ ## param_name->values)[_unicorn_test_param_ ## param_name->index]; \
    })


#endif

// #include "unicorn/result/result.h"
#ifndef UNICORN_RESULT_H
#define UNICORN_RESULT_H


#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>

// #include "unicorn/types.h"



struct UnicornTestResult
{
    bool success;
    char *failed_assertion;
    char *error_message;
    char *assertion_file;
    size_t assertion_line;
    UnicornTest *test;
    UnicornCollection *param_snapshots;
    struct timeval start_time;
    struct timeval end_time;
    int pipe[2];
    int output_pipe[2];
    char *output_buffer;
    size_t output_length;
    char *diff_original;
    size_t diff_original_size;
    char *diff_modified;
    size_t diff_modified_size;
};

UnicornTestResult *unicorn_new_test_result();

bool unicorn_test_result_has_diff(UnicornTestResult *test_result);

void unicorn_pipe_test_info(UnicornTestResult *test_result, struct timeval start_time, struct timeval end_time);
void unicorn_pipe_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, char *assertion_file, size_t assertion_line);
void unicorn_pipe_error_message(UnicornTestResult *test_result, char *error_message, size_t message_size);

void unicorn_set_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, char *assertion_file, size_t assertion_line);
void unicorn_set_error_message(UnicornTestResult *test_result, char *error_message, size_t message_size);
void unicorn_free_test_result(UnicornTestResult *test_result);


struct UnicornTestParamSnapshot
{
    UnicornTestParam *param;
    size_t index;
};

UnicornTestParamSnapshot *unicorn_new_test_param_snapshot(UnicornTestParam *test_param);
void unicorn_free_test_param_snapshot(UnicornTestParamSnapshot *param_snapshot);


#endif

// #include "unicorn/session/session.h"
#ifndef UNICORN_SESSION_H
#define UNICORN_SESSION_H


#include <sys/time.h>

// #include "unicorn/types.h"



struct UnicornSessionOutputState
{
    int index;
    int length;
    char string[50];
};

struct UnicornTestSession
{
    UnicornCollection *results;
    UnicornCollection *failures;
    struct timeval start_time;
    struct timeval end_time;
    UnicornSessionOutputState output_state;
};

UnicornTestSession *unicorn_new_test_session();

void unicorn_test_session_start(UnicornTestSession *test_session);
void unicorn_test_session_end(UnicornTestSession *test_session);

void unicorn_test_session_run_test(UnicornTestSession *test_session, UnicornTest *test);
void unicorn_test_session_run_parameterized_test(UnicornTestSession *test_session, UnicornTest *test, UnicornCollectionItem *param_item);
void unicorn_test_session_run_test_group(UnicornTestSession *test_session, UnicornTestGroup *test_group);

void unicorn_free_test_session(UnicornTestSession *test_session);


#endif

// #include "unicorn/test/test.h"
#ifndef UNICORN_TEST_H
#define UNICORN_TEST_H


#include <stdlib.h>

// #include "unicorn/types.h"



extern UnicornTest *_unicorn_current_test;


struct UnicornTest
{
    char *name;
    char *filename;
    size_t line_number;
    UnicornTestGroup *group;
    UnicornTestFunction function;
    UnicornCollection *resources;
    UnicornCollection *fixtures;
    UnicornCollection *params;
    UnicornCollection *accessible_fixtures;
    UnicornCollection *accessible_params;
    UnicornTestResult *result;
    UnicornOutputCapture *output_capture;
};

UnicornTest *unicorn_new_test(char *name, char *filename, size_t line_number, UnicornTestFunction function, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);
void unicorn_run_test(UnicornTest *test);

void unicorn_free_after_test(UnicornTest *test, void *resource);
void auto_free(void *resource);
void *unicorn_test_resource(UnicornTest *test, size_t size);
void *test_resource(size_t size);
void unicorn_free_test_resources(UnicornTest *test);

void unicorn_register_test_fixture(UnicornTest *test, UnicornCollection *access_collection, char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);
void unicorn_register_test_param(UnicornTest *test, UnicornCollection *access_collection, char *name, void *values, size_t count);

void unicorn_free_test(UnicornTest *test);


#define DECLARE_TEST(test_name) \
    extern UnicornTestModifierRegistration _unicorn_test_modifiers_ ## test_name[]; \
    void _unicorn_test_function_ ## test_name(); \
    void test_name(UnicornTestGroup *test_group)


#define TEST(test_name, ...) \
    DECLARE_TEST(test_name); \
    UnicornTestModifierRegistration _unicorn_test_modifiers_ ## test_name[] = { __VA_ARGS__ }; \
    void test_name(UnicornTestGroup *test_group) \
    { \
        unicorn_register_test(test_group, #test_name, __FILE__, __LINE__, _unicorn_test_function_ ## test_name, _unicorn_test_modifiers_ ## test_name, sizeof (_unicorn_test_modifiers_ ## test_name) / sizeof (*_unicorn_test_modifiers_ ## test_name)); \
    } \
    void _unicorn_test_function_ ## test_name()


#endif

// #include "unicorn/test_utils/test_utils.h"
#ifndef UNICORN_TEST_UTILS_H
#define UNICORN_TEST_UTILS_H


#include <stdbool.h>

// #include "unicorn/types.h"



extern UnicornOutputCapture _unicorn_default_output_capture;


struct UnicornOutputCapture
{
    bool initialization_phase;
    int stdout_backup;
    int stderr_backup;
    int pipe[2];
    UnicornOutputCapture *parent;
};

bool unicorn_capture_output(UnicornOutputCapture *capture, char **output_buffer);


#define CAPTURE_OUTPUT(buffer_name) \
    char *buffer_name = NULL; \
    UnicornOutputCapture _unicorn_capture_ ## buffer_name = _unicorn_default_output_capture; \
    while (unicorn_capture_output(&_unicorn_capture_ ## buffer_name, &buffer_name))


#endif



int unicorn_run_root_group(UnicornGroupItemRegistration *root_items, size_t item_count);


#define RUN_TESTS(...) \
    ({ \
        UnicornGroupItemRegistration _unicorn_root_items[] = { __VA_ARGS__ }; \
        unicorn_run_root_group(_unicorn_root_items, sizeof (_unicorn_root_items) / sizeof (*_unicorn_root_items)); \
    })


#endif
