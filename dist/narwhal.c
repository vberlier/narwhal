/*
Narwhal v0.2.1 (https://github.com/vberlier/narwhal)
Amalgamated source file

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

#define _XOPEN_SOURCE 700

// #include "narwhal/test/test.h"
#ifndef NARWHAL_TEST_H
#define NARWHAL_TEST_H

#include <stdlib.h>

// #include "narwhal/types.h"
#ifndef NARWHAL_TYPES_H
#define NARWHAL_TYPES_H

// #include "narwhal/collection/types.h"
#ifndef NARWHAL_COLLECTION_TYPES_H
#define NARWHAL_COLLECTION_TYPES_H

typedef struct NarwhalCollection NarwhalCollection;
typedef struct NarwhalCollectionItem NarwhalCollectionItem;

#endif

// #include "narwhal/diff/types.h"
#ifndef NARWHAL_DIFF_TYPES_H
#define NARWHAL_DIFF_TYPES_H

typedef struct NarwhalDiffMatrix NarwhalDiffMatrix;
typedef enum NarwhalDiffChunkType NarwhalDiffChunkType;
typedef struct NarwhalDiff NarwhalDiff;
typedef struct NarwhalDiffChunk NarwhalDiffChunk;

#endif

// #include "narwhal/fixture/types.h"
#ifndef NARWHAL_FIXTURE_TYPES_H
#define NARWHAL_FIXTURE_TYPES_H

typedef struct NarwhalTestFixture NarwhalTestFixture;

typedef void (*NarwhalTestFixtureSetup)(void *value, NarwhalTestFixture *test_fixture);
typedef void (*NarwhalTestFixtureCleanup)(void *value, NarwhalTestFixture *test_fixture);

#endif

// #include "narwhal/group/types.h"
#ifndef NARWHAL_GROUP_TYPES_H
#define NARWHAL_GROUP_TYPES_H

typedef struct NarwhalTestGroup NarwhalTestGroup;

typedef void (*NarwhalGroupItemRegistration)(NarwhalTestGroup *test_group);

#endif

// #include "narwhal/param/types.h"
#ifndef NARWHAL_PARAM_TYPES_H
#define NARWHAL_PARAM_TYPES_H

typedef struct NarwhalTestParam NarwhalTestParam;

#endif

// #include "narwhal/result/types.h"
#ifndef NARWHAL_RESULT_TYPES_H
#define NARWHAL_RESULT_TYPES_H

typedef struct NarwhalTestResult NarwhalTestResult;
typedef struct NarwhalTestParamSnapshot NarwhalTestParamSnapshot;

#endif

// #include "narwhal/session/types.h"
#ifndef NARWHAL_SESSION_TYPES_H
#define NARWHAL_SESSION_TYPES_H

typedef struct NarwhalTestSession NarwhalTestSession;
typedef struct NarwhalSessionOutputState NarwhalSessionOutputState;

#endif

// #include "narwhal/test/types.h"
#ifndef NARWHAL_TEST_TYPES_H
#define NARWHAL_TEST_TYPES_H

typedef struct NarwhalTest NarwhalTest;

typedef void (*NarwhalTestModifierRegistration)(NarwhalTest *test,
                                                NarwhalCollection *params,
                                                NarwhalCollection *fixtures);
typedef void (*NarwhalTestFunction)();

#endif

// #include "narwhal/test_utils/types.h"
#ifndef NARWHAL_TEST_UTILS_TYPES_H
#define NARWHAL_TEST_UTILS_TYPES_H

typedef struct NarwhalOutputCapture NarwhalOutputCapture;

#endif


#endif


extern NarwhalTest *_narwhal_current_test;

struct NarwhalTest
{
    char *name;
    char *filename;
    size_t line_number;
    NarwhalTestGroup *group;
    NarwhalTestFunction function;
    NarwhalCollection *resources;
    NarwhalCollection *fixtures;
    NarwhalCollection *params;
    NarwhalCollection *accessible_fixtures;
    NarwhalCollection *accessible_params;
    NarwhalTestResult *result;
    NarwhalOutputCapture *output_capture;
};

NarwhalTest *narwhal_new_test(char *name,
                              char *filename,
                              size_t line_number,
                              NarwhalTestFunction function,
                              NarwhalTestModifierRegistration *test_modifiers,
                              size_t modifier_count);
void narwhal_run_test(NarwhalTest *test);

void narwhal_free_after_test(NarwhalTest *test, void *resource);
void auto_free(void *resource);
void *narwhal_test_resource(NarwhalTest *test, size_t size);
void *test_resource(size_t size);
void narwhal_free_test_resources(NarwhalTest *test);

void narwhal_register_test_fixture(NarwhalTest *test,
                                   NarwhalCollection *access_collection,
                                   char *name,
                                   size_t fixture_size,
                                   NarwhalTestFixtureSetup setup,
                                   NarwhalTestModifierRegistration *test_modifiers,
                                   size_t modifier_count);
void narwhal_register_test_param(NarwhalTest *test,
                                 NarwhalCollection *access_collection,
                                 char *name,
                                 void *values,
                                 size_t count);

void narwhal_free_test(NarwhalTest *test);

#define DECLARE_TEST(test_name)                                                   \
    extern NarwhalTestModifierRegistration _narwhal_test_modifiers_##test_name[]; \
    void _narwhal_test_function_##test_name();                                    \
    void test_name(NarwhalTestGroup *test_group)

#define TEST(test_name, ...)                                                                 \
    DECLARE_TEST(test_name);                                                                 \
    NarwhalTestModifierRegistration _narwhal_test_modifiers_##test_name[] = { __VA_ARGS__ }; \
    void test_name(NarwhalTestGroup *test_group)                                             \
    {                                                                                        \
        narwhal_register_test(test_group,                                                    \
                              #test_name,                                                    \
                              __FILE__,                                                      \
                              __LINE__,                                                      \
                              _narwhal_test_function_##test_name,                            \
                              _narwhal_test_modifiers_##test_name,                           \
                              sizeof(_narwhal_test_modifiers_##test_name) /                  \
                                  sizeof(*_narwhal_test_modifiers_##test_name));             \
    }                                                                                        \
    void _narwhal_test_function_##test_name()

#endif


#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

// #include "narwhal/collection/collection.h"
#ifndef NARWHAL_COLLECTION_H
#define NARWHAL_COLLECTION_H

#include <stdbool.h>
#include <stdlib.h>

// #include "narwhal/types.h"


struct NarwhalCollection
{
    size_t count;
    NarwhalCollectionItem *first;
    NarwhalCollectionItem *last;
};

struct NarwhalCollectionItem
{
    void *value;
    NarwhalCollectionItem *previous;
    NarwhalCollectionItem *next;
};

NarwhalCollection *narwhal_empty_collection();
void narwhal_collection_append(NarwhalCollection *collection, void *value);
void *narwhal_collection_pop(NarwhalCollection *collection);
void narwhal_free_collection(NarwhalCollection *collection);

// Foreach macros

#define NARWHAL_EACH(item_variable, collection)                                             \
    for (NarwhalCollectionItem *_narwhal_loop_item = (collection)->first;                   \
         _narwhal_loop_item != NULL && ((item_variable) = _narwhal_loop_item->value, true); \
         _narwhal_loop_item = _narwhal_loop_item->next)

#define NARWHAL_REVERSED(item_variable, collection)                                         \
    for (NarwhalCollectionItem *_narwhal_loop_item = (collection)->last;                    \
         _narwhal_loop_item != NULL && ((item_variable) = _narwhal_loop_item->value, true); \
         _narwhal_loop_item = _narwhal_loop_item->previous)

#endif

// #include "narwhal/fixture/fixture.h"
#ifndef NARWHAL_FIXTURE_H
#define NARWHAL_FIXTURE_H

#include <stdlib.h>

// #include "narwhal/types.h"


extern NarwhalCollection *_narwhal_current_fixtures;

struct NarwhalTestFixture
{
    char *name;
    size_t size;
    void *value;
    NarwhalTestFixtureSetup setup;
    NarwhalTestFixtureCleanup cleanup;
    NarwhalTest *test;
    NarwhalCollection *accessible_fixtures;
    NarwhalCollection *accessible_params;
};

NarwhalTestFixture *narwhal_new_test_fixture(char *name,
                                             size_t fixture_size,
                                             NarwhalTestFixtureSetup setup,
                                             NarwhalTest *test,
                                             NarwhalTestModifierRegistration *test_modifiers,
                                             size_t modifier_count);
NarwhalTestFixture *narwhal_get_test_fixture(NarwhalCollection *fixtures, char *fixture_name);
void narwhal_free_test_fixture(NarwhalTestFixture *test_fixture);

#define DECLARE_FIXTURE(fixture_name, fixture_type)                                          \
    typedef fixture_type _narwhal_fixture_type_##fixture_name;                               \
    extern NarwhalTestModifierRegistration _narwhal_test_fixture_modifiers_##fixture_name[]; \
    void _narwhal_fixture_##fixture_name##_setup(                                            \
        _narwhal_fixture_type_##fixture_name *fixture_name,                                  \
        NarwhalTestFixture *_narwhal_test_fixture);                                          \
    void _narwhal_fixture_##fixture_name##_call_setup(void *value,                           \
                                                      NarwhalTestFixture *test_fixture);     \
    void _narwhal_fixture_##fixture_name##_cleanup(                                          \
        _narwhal_fixture_type_##fixture_name *fixture_name,                                  \
        NarwhalTestFixture *_narwhal_test_fixture);                                          \
    void _narwhal_fixture_##fixture_name##_call_cleanup(void *value,                         \
                                                        NarwhalTestFixture *test_fixture);   \
    void fixture_name(NarwhalTest *test, NarwhalCollection *params, NarwhalCollection *fixtures)

#define TEST_FIXTURE(fixture_name, fixture_type, ...)                                          \
    DECLARE_FIXTURE(fixture_name, fixture_type);                                               \
    NarwhalTestModifierRegistration _narwhal_test_fixture_modifiers_##fixture_name[] = {       \
        __VA_ARGS__                                                                            \
    };                                                                                         \
    void _narwhal_fixture_##fixture_name##_call_setup(void *value,                             \
                                                      NarwhalTestFixture *test_fixture)        \
    {                                                                                          \
        _narwhal_fixture_##fixture_name##_setup((_narwhal_fixture_type_##fixture_name *)value, \
                                                test_fixture);                                 \
    }                                                                                          \
    void fixture_name(                                                                         \
        NarwhalTest *test, UNUSED NarwhalCollection *params, NarwhalCollection *fixtures)      \
    {                                                                                          \
        narwhal_register_test_fixture(                                                         \
            test,                                                                              \
            fixtures,                                                                          \
            #fixture_name,                                                                     \
            sizeof(_narwhal_fixture_type_##fixture_name),                                      \
            _narwhal_fixture_##fixture_name##_call_setup,                                      \
            _narwhal_test_fixture_modifiers_##fixture_name,                                    \
            sizeof(_narwhal_test_fixture_modifiers_##fixture_name) /                           \
                sizeof(*_narwhal_test_fixture_modifiers_##fixture_name));                      \
    }                                                                                          \
    void _narwhal_fixture_##fixture_name##_setup(                                              \
        UNUSED _narwhal_fixture_type_##fixture_name *fixture_name,                             \
        UNUSED NarwhalTestFixture *_narwhal_test_fixture)

#define GET_FIXTURE(fixture_name)                                                              \
    _narwhal_fixture_type_##fixture_name fixture_name = ({                                     \
        NarwhalTestFixture *_narwhal_test_fixture_##fixture_name =                             \
            narwhal_get_test_fixture(_narwhal_current_fixtures, #fixture_name);                \
        if (_narwhal_test_fixture_##fixture_name == NULL)                                      \
            FAIL("Fixture \"%s\" hasn't been applied to the current context.", #fixture_name); \
        *(_narwhal_fixture_type_##fixture_name *)_narwhal_test_fixture_##fixture_name->value;  \
    })

#define CLEANUP_FIXTURE(fixture_name)                                                            \
    _narwhal_test_fixture->cleanup = _narwhal_fixture_##fixture_name##_call_cleanup;             \
    }                                                                                            \
    void _narwhal_fixture_##fixture_name##_call_cleanup(void *value,                             \
                                                        NarwhalTestFixture *test_fixture)        \
    {                                                                                            \
        _narwhal_fixture_##fixture_name##_cleanup((_narwhal_fixture_type_##fixture_name *)value, \
                                                  test_fixture);                                 \
    }                                                                                            \
    void _narwhal_fixture_##fixture_name##_cleanup(                                              \
        UNUSED _narwhal_fixture_type_##fixture_name *fixture_name,                               \
        UNUSED NarwhalTestFixture *_narwhal_test_fixture)                                        \
    {
#endif

// #include "narwhal/param/param.h"
#ifndef NARWHAL_PARAM_H
#define NARWHAL_PARAM_H

#include <stdlib.h>

// #include "narwhal/types.h"


extern NarwhalCollection *_narwhal_current_params;

struct NarwhalTestParam
{
    char *name;
    size_t index;
    size_t count;
    void *values;
    NarwhalTest *test;
};

NarwhalTestParam *narwhal_new_test_param(char *name,
                                         void *values,
                                         size_t count,
                                         NarwhalTest *test);
NarwhalTestParam *narwhal_get_test_param(NarwhalCollection *params, char *param_name);
void narwhal_free_test_param(NarwhalTestParam *test_param);

#define DECLARE_PARAM(param_name, param_type)                              \
    typedef param_type _narwhal_param_type_##param_name;                   \
    extern _narwhal_param_type_##param_name _narwhal_param_##param_name[]; \
    void param_name(NarwhalTest *test, NarwhalCollection *params, NarwhalCollection *fixtures)

#define TEST_PARAM(param_name, param_type, ...)                                           \
    DECLARE_PARAM(param_name, param_type);                                                \
    _narwhal_param_type_##param_name _narwhal_param_##param_name[] = __VA_ARGS__;         \
    void param_name(                                                                      \
        NarwhalTest *test, NarwhalCollection *params, UNUSED NarwhalCollection *fixtures) \
    {                                                                                     \
        narwhal_register_test_param(                                                      \
            test,                                                                         \
            params,                                                                       \
            #param_name,                                                                  \
            _narwhal_param_##param_name,                                                  \
            sizeof(_narwhal_param_##param_name) / sizeof(*_narwhal_param_##param_name));  \
    }

#define GET_PARAM(param_name)                                                                    \
    _narwhal_param_type_##param_name param_name = ({                                             \
        NarwhalTestParam *_narwhal_test_param_##param_name =                                     \
            narwhal_get_test_param(_narwhal_current_params, #param_name);                        \
        if (_narwhal_test_param_##param_name == NULL)                                            \
            FAIL("Parameter \"%s\" hasn't been applied to the current context.", #param_name);   \
        ((_narwhal_param_type_##param_name *)                                                    \
             _narwhal_test_param_##param_name->values)[_narwhal_test_param_##param_name->index]; \
    })

#endif

// #include "narwhal/result/result.h"
#ifndef NARWHAL_RESULT_H
#define NARWHAL_RESULT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

// #include "narwhal/types.h"


struct NarwhalTestResult
{
    bool success;
    char *failed_assertion;
    char *error_message;
    char *assertion_file;
    size_t assertion_line;
    NarwhalTest *test;
    NarwhalCollection *param_snapshots;
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

NarwhalTestResult *narwhal_new_test_result();

bool narwhal_test_result_has_diff(NarwhalTestResult *test_result);

void narwhal_pipe_test_info(NarwhalTestResult *test_result,
                            struct timeval start_time,
                            struct timeval end_time);
void narwhal_pipe_assertion_failure(NarwhalTestResult *test_result,
                                    char *failed_assertion,
                                    char *assertion_file,
                                    size_t assertion_line);
void narwhal_pipe_error_message(NarwhalTestResult *test_result,
                                char *error_message,
                                size_t message_size);

void narwhal_set_assertion_failure(NarwhalTestResult *test_result,
                                   char *failed_assertion,
                                   char *assertion_file,
                                   size_t assertion_line);
void narwhal_set_error_message(NarwhalTestResult *test_result,
                               char *error_message,
                               size_t message_size);
void narwhal_free_test_result(NarwhalTestResult *test_result);

struct NarwhalTestParamSnapshot
{
    NarwhalTestParam *param;
    size_t index;
};

NarwhalTestParamSnapshot *narwhal_new_test_param_snapshot(NarwhalTestParam *test_param);
void narwhal_free_test_param_snapshot(NarwhalTestParamSnapshot *param_snapshot);

#endif

// #include "narwhal/utils.h"
#ifndef NARWHAL_UTILS_H
#define NARWHAL_UTILS_H

#include <stdbool.h>
#include <stdio.h>

size_t narwhal_util_read_stream(FILE *stream, char **buffer);
bool narwhal_is_short_string(char *string);
int narwhal_min_int(int a, int b);
size_t narwhal_min_size_t(size_t a, size_t b);
size_t narwhal_count_chars(char *string, char chr);
char *narwhal_next_line(char *string);
char *narwhal_next_lines(char *string, size_t lines);

#endif


/*
 * Current test
 */

NarwhalTest *_narwhal_current_test = NULL;

/*
 * Test creation
 */

static void initialize_test(NarwhalTest *test,
                            char *name,
                            char *filename,
                            size_t line_number,
                            NarwhalTestFunction function,
                            NarwhalTestModifierRegistration *test_modifiers,
                            size_t modifier_count)
{
    test->name = name;
    test->filename = filename;
    test->line_number = line_number;
    test->group = NULL;
    test->function = function;
    test->resources = narwhal_empty_collection();
    test->fixtures = narwhal_empty_collection();
    test->params = narwhal_empty_collection();
    test->accessible_fixtures = narwhal_empty_collection();
    test->accessible_params = narwhal_empty_collection();
    test->result = NULL;
    test->output_capture = NULL;

    for (size_t i = 0; i < modifier_count; i++)
    {
        NarwhalTestModifierRegistration registration = test_modifiers[i];
        registration(test, test->accessible_params, test->accessible_fixtures);
    }
}

NarwhalTest *narwhal_new_test(char *name,
                              char *filename,
                              size_t line_number,
                              NarwhalTestFunction function,
                              NarwhalTestModifierRegistration *test_modifiers,
                              size_t modifier_count)
{
    NarwhalTest *test = malloc(sizeof(NarwhalTest));
    initialize_test(test, name, filename, line_number, function, test_modifiers, modifier_count);

    return test;
}

/*
 * Test resources
 */

void narwhal_free_after_test(NarwhalTest *test, void *resource)
{
    narwhal_collection_append(test->resources, resource);
}

void auto_free(void *resource)
{
    narwhal_free_after_test(_narwhal_current_test, resource);
}

void *narwhal_test_resource(NarwhalTest *test, size_t size)
{
    void *resource = malloc(size);
    narwhal_free_after_test(test, resource);
    return resource;
}

void *test_resource(size_t size)
{
    return narwhal_test_resource(_narwhal_current_test, size);
}

void narwhal_free_test_resources(NarwhalTest *test)
{
    while (test->resources->count > 0)
    {
        void *test_resource = narwhal_collection_pop(test->resources);
        free(test_resource);
    }
}

/*
 * Report test data
 */

#define pull_data(value, size)                                          \
    if (read(test_result->pipe[0], (value), (size)) != (ssize_t)(size)) \
    fprintf(stderr, "%s:%d: Failed to read from result pipe.\n", __FILE__, __LINE__)

static void test_error(NarwhalTestResult *test_result, char *message, size_t message_size)
{
    narwhal_set_assertion_failure(
        test_result, NULL, test_result->test->filename, test_result->test->line_number);
    narwhal_set_error_message(test_result, message, message_size);
}

static void report_success(NarwhalTestResult *test_result)
{
    bool test_success;
    ssize_t bytes_read = read(test_result->pipe[0], &test_success, sizeof(bool));

    if (bytes_read != sizeof(bool))
    {
        char message[] = "Test process exited unexpectedly.";
        test_error(test_result, message, sizeof(message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    pull_data(&test_result->start_time, sizeof(struct timeval));
    pull_data(&test_result->end_time, sizeof(struct timeval));
}

static void report_failure(NarwhalTestResult *test_result)
{
    bool test_success;
    ssize_t bytes_read = read(test_result->pipe[0], &test_success, sizeof(bool));

    if (bytes_read != sizeof(bool))
    {
        char message[] = "Test process exited unexpectedly.";
        test_error(test_result, message, sizeof(message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    if (test_success)
    {
        pull_data(&test_result->start_time, sizeof(struct timeval));
        pull_data(&test_result->end_time, sizeof(struct timeval));

        char message[] = "Test process exited with non-zero return code.";
        test_error(test_result, message, sizeof(message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    size_t assertion_size;
    pull_data(&assertion_size, sizeof(size_t));

    if (assertion_size > 0)
    {
        test_result->failed_assertion = malloc(assertion_size);
        pull_data(test_result->failed_assertion, assertion_size);
    }
    else
    {
        test_result->failed_assertion = NULL;
    }

    size_t filename_size;
    pull_data(&filename_size, sizeof(size_t));

    test_result->assertion_file = malloc(filename_size);
    pull_data(test_result->assertion_file, filename_size);

    pull_data(&test_result->assertion_line, sizeof(size_t));

    bool has_diff;
    pull_data(&has_diff, sizeof(has_diff));

    if (has_diff)
    {
        pull_data(&test_result->diff_original_size, sizeof(test_result->diff_original_size));
        test_result->diff_original = malloc(test_result->diff_original_size);
        pull_data(test_result->diff_original, test_result->diff_original_size);

        pull_data(&test_result->diff_modified_size, sizeof(test_result->diff_modified_size));
        test_result->diff_modified = malloc(test_result->diff_modified_size);
        pull_data(test_result->diff_modified, test_result->diff_modified_size);
    }

    size_t message_size;
    pull_data(&message_size, sizeof(size_t));

    test_result->error_message = malloc(message_size);
    pull_data(test_result->error_message, message_size);

    pull_data(&test_result->start_time, sizeof(struct timeval));
    pull_data(&test_result->end_time, sizeof(struct timeval));
}

static void report_output(NarwhalTestResult *test_result)
{
    FILE *stream = fdopen(test_result->output_pipe[0], "r");

    test_result->output_length = narwhal_util_read_stream(stream, &test_result->output_buffer);

    fclose(stream);
}

#undef pull_data

/*
 * Run test
 */

static void setup_test_result(NarwhalTest *test)
{
    test->result = narwhal_new_test_result();
    test->result->test = test;

    NarwhalTestParam *test_param;
    NARWHAL_EACH(test_param, test->params)
    {
        NarwhalTestParamSnapshot *param_snapshot = narwhal_new_test_param_snapshot(test_param);
        narwhal_collection_append(test->result->param_snapshots, param_snapshot);
    }
}

static int test_start(NarwhalTest *test)
{
    bool test_success = test->result->success;

    NarwhalTestFixture *test_fixture;
    NARWHAL_EACH(test_fixture, test->fixtures)
    {
        test_fixture->cleanup = NULL;
        test_fixture->value = narwhal_test_resource(test, test_fixture->size);

        _narwhal_current_test = test;
        _narwhal_current_params = test_fixture->accessible_params;
        _narwhal_current_fixtures = test_fixture->accessible_fixtures;

        test_fixture->setup(test_fixture->value, test_fixture);

        _narwhal_current_test = NULL;
        _narwhal_current_params = NULL;
        _narwhal_current_fixtures = NULL;

        fflush(stdout);
        fflush(stderr);

        if (test->result->success != test_success)
        {
            break;
        }
    }

    return test->result->success == test_success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int test_end(NarwhalTest *test)
{
    bool test_success = test->result->success;

    NarwhalTestFixture *test_fixture;
    NARWHAL_REVERSED(test_fixture, test->fixtures)
    {
        if (test_fixture->cleanup != NULL)
        {
            _narwhal_current_test = test;
            _narwhal_current_params = test_fixture->accessible_params;
            _narwhal_current_fixtures = test_fixture->accessible_fixtures;

            test_fixture->cleanup(test_fixture->value, test_fixture);

            _narwhal_current_test = NULL;
            _narwhal_current_params = NULL;
            _narwhal_current_fixtures = NULL;

            fflush(stdout);
            fflush(stderr);

            if (test->result->success != test_success)
            {
                break;
            }
        }
    }

    narwhal_free_test_resources(test);

    return test->result->success == test_success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int execute_test_function(NarwhalTest *test)
{
    struct timeval start_time;
    struct timeval end_time;

    gettimeofday(&start_time, NULL);

    if (test_start(test) == EXIT_FAILURE)
    {
        narwhal_pipe_test_info(test->result, start_time, start_time);
        return EXIT_FAILURE;
    }

    _narwhal_current_test = test;
    _narwhal_current_params = test->accessible_params;
    _narwhal_current_fixtures = test->accessible_fixtures;

    gettimeofday(&start_time, NULL);

    test->function();

    gettimeofday(&end_time, NULL);

    _narwhal_current_test = NULL;
    _narwhal_current_params = NULL;
    _narwhal_current_fixtures = NULL;

    fflush(stdout);
    fflush(stderr);

    if (test->result->success)
    {
        test_end(test);
        narwhal_pipe_test_info(test->result, start_time, end_time);
    }
    else
    {
        narwhal_pipe_test_info(test->result, start_time, end_time);
        test_end(test);
    }

    return test->result->success ? EXIT_SUCCESS : EXIT_FAILURE;
}

void narwhal_run_test(NarwhalTest *test)
{
    setup_test_result(test);

    NarwhalTestResult *test_result = test->result;

    if (pipe(test_result->pipe) == -1)
    {
        char message[] = "Couldn't create the test pipe.";
        test_error(test_result, message, sizeof(message));
        return;
    }

    if (pipe(test_result->output_pipe) == -1)
    {
        char message[] = "Couldn't create the output pipe.";
        test_error(test_result, message, sizeof(message));

        close(test_result->pipe[0]);
        close(test_result->pipe[1]);

        return;
    }

    pid_t test_pid = fork();

    if (test_pid == -1)
    {
        char message[] = "Couldn't create the test child process.";
        test_error(test_result, message, sizeof(message));

        close(test_result->pipe[0]);
        close(test_result->pipe[1]);
        close(test_result->output_pipe[0]);
        close(test_result->output_pipe[1]);

        return;
    }
    else if (test_pid == 0)
    {
        close(test_result->pipe[0]);

        while (dup2(test_result->output_pipe[1], STDOUT_FILENO) == -1 && errno == EINTR)
            ;
        while (dup2(test_result->output_pipe[1], STDERR_FILENO) == -1 && errno == EINTR)
            ;
        close(test_result->output_pipe[0]);
        close(test_result->output_pipe[1]);

        int test_status = execute_test_function(test);

        close(test_result->pipe[1]);
        exit(test_status);
    }

    gettimeofday(&test_result->start_time, NULL);

    close(test_result->pipe[1]);
    close(test_result->output_pipe[1]);

    int test_status;
    waitpid(test_pid, &test_status, 0);

    test_result->success = test_status == EXIT_SUCCESS;

    if (test_result->success)
    {
        report_success(test_result);
    }
    else
    {
        report_failure(test_result);
    }

    report_output(test_result);

    close(test_result->output_pipe[0]);
    close(test_result->pipe[0]);
}

/*
 * Register test modifiers
 */

void narwhal_register_test_fixture(NarwhalTest *test,
                                   NarwhalCollection *access_collection,
                                   char *name,
                                   size_t fixture_size,
                                   NarwhalTestFixtureSetup setup,
                                   NarwhalTestModifierRegistration *test_modifiers,
                                   size_t modifier_count)
{
    NarwhalTestFixture *test_fixture = narwhal_get_test_fixture(test->fixtures, name);

    if (test_fixture == NULL)
    {
        test_fixture = narwhal_new_test_fixture(
            name, fixture_size, setup, test, test_modifiers, modifier_count);

        narwhal_collection_append(test->fixtures, test_fixture);
        narwhal_collection_append(access_collection, test_fixture);
    }
    else if (narwhal_get_test_fixture(access_collection, name) == NULL)
    {
        narwhal_collection_append(access_collection, test_fixture);
    }
}

void narwhal_register_test_param(NarwhalTest *test,
                                 NarwhalCollection *access_collection,
                                 char *name,
                                 void *values,
                                 size_t count)
{
    NarwhalTestParam *test_param = narwhal_get_test_param(test->params, name);

    if (test_param == NULL)
    {
        test_param = narwhal_new_test_param(name, values, count, test);

        narwhal_collection_append(test->params, test_param);
        narwhal_collection_append(access_collection, test_param);
    }
    else if (narwhal_get_test_param(access_collection, name) == NULL)
    {
        narwhal_collection_append(access_collection, test_param);
    }
}

/*
 * Cleanup
 */

void narwhal_free_test(NarwhalTest *test)
{
    while (test->accessible_fixtures->count > 0)
    {
        narwhal_collection_pop(test->accessible_fixtures);
    }
    narwhal_free_collection(test->accessible_fixtures);

    while (test->accessible_params->count > 0)
    {
        narwhal_collection_pop(test->accessible_params);
    }
    narwhal_free_collection(test->accessible_params);

    while (test->fixtures->count > 0)
    {
        NarwhalTestFixture *test_fixture = narwhal_collection_pop(test->fixtures);
        narwhal_free_test_fixture(test_fixture);
    }
    narwhal_free_collection(test->fixtures);

    while (test->params->count > 0)
    {
        NarwhalTestParam *test_param = narwhal_collection_pop(test->params);
        narwhal_free_test_param(test_param);
    }
    narwhal_free_collection(test->params);

    narwhal_free_collection(test->resources);

    free(test);
}
// #include "narwhal/result/result.h"


#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

// #include "narwhal/collection/collection.h"

// #include "narwhal/param/param.h"

// #include "narwhal/test/test.h"

// #include "narwhal/test_utils/test_utils.h"
#ifndef NARWHAL_TEST_UTILS_H
#define NARWHAL_TEST_UTILS_H

#include <stdbool.h>

// #include "narwhal/types.h"


extern NarwhalOutputCapture _narwhal_default_output_capture;

struct NarwhalOutputCapture
{
    bool initialization_phase;
    int stdout_backup;
    int stderr_backup;
    int pipe[2];
    NarwhalOutputCapture *parent;
};

bool narwhal_capture_output(NarwhalOutputCapture *capture, char **output_buffer);

#define CAPTURE_OUTPUT(buffer_name)                                                        \
    char *buffer_name = NULL;                                                              \
    NarwhalOutputCapture _narwhal_capture_##buffer_name = _narwhal_default_output_capture; \
    while (narwhal_capture_output(&_narwhal_capture_##buffer_name, &buffer_name))

#endif


/*
 * Test result initialization
 */

static void initialize_test_result(NarwhalTestResult *test_result)
{
    test_result->success = true;
    test_result->failed_assertion = NULL;
    test_result->error_message = NULL;
    test_result->assertion_file = NULL;
    test_result->assertion_line = 0;
    test_result->test = NULL;
    test_result->param_snapshots = narwhal_empty_collection();
    test_result->output_buffer = NULL;
    test_result->output_length = 0;
    test_result->diff_original = NULL;
    test_result->diff_original_size = -1;
    test_result->diff_modified = NULL;
    test_result->diff_modified_size = -1;
}

NarwhalTestResult *narwhal_new_test_result()
{
    NarwhalTestResult *test_result = malloc(sizeof(NarwhalTestResult));
    initialize_test_result(test_result);

    return test_result;
}

/*
 * Util
 */

bool narwhal_test_result_has_diff(NarwhalTestResult *test_result)
{
    return test_result->diff_original != NULL && test_result->diff_original_size > 0 &&
           test_result->diff_modified != NULL && test_result->diff_modified_size > 0;
}

/*
 * Pipe test result data
 */

#define push_data(value, size)                                           \
    if (write(test_result->pipe[1], (value), (size)) != (ssize_t)(size)) \
    fprintf(stderr, "Failed to write to result pipe.\n")

void narwhal_pipe_test_info(NarwhalTestResult *test_result,
                            struct timeval start_time,
                            struct timeval end_time)
{
    if (test_result->success)
    {
        push_data(&test_result->success, sizeof(bool));
    }

    push_data(&start_time, sizeof(start_time));
    push_data(&end_time, sizeof(end_time));
}

void narwhal_pipe_assertion_failure(NarwhalTestResult *test_result,
                                    char *failed_assertion,
                                    char *assertion_file,
                                    size_t assertion_line)
{
    while (test_result->test->output_capture != NULL)
    {
        char *output_buffer = NULL;
        while (narwhal_capture_output(test_result->test->output_capture, &output_buffer))
            ;
    }

    test_result->success = false;
    push_data(&test_result->success, sizeof(bool));

    size_t assertion_size = failed_assertion != NULL ? strlen(failed_assertion) + 1 : 0;
    push_data(&assertion_size, sizeof(assertion_size));
    push_data(failed_assertion, assertion_size);

    size_t filename_size = strlen(assertion_file) + 1;
    push_data(&filename_size, sizeof(filename_size));
    push_data(assertion_file, filename_size);

    push_data(&assertion_line, sizeof(assertion_line));

    bool has_diff = false;

    if (narwhal_test_result_has_diff(test_result))
    {
        has_diff = true;
        push_data(&has_diff, sizeof(has_diff));

        push_data(&test_result->diff_original_size, sizeof(test_result->diff_original_size));
        push_data(test_result->diff_original, test_result->diff_original_size);

        push_data(&test_result->diff_modified_size, sizeof(test_result->diff_modified_size));
        push_data(test_result->diff_modified, test_result->diff_modified_size);
    }
    else
    {
        push_data(&has_diff, sizeof(has_diff));
    }
}

void narwhal_pipe_error_message(NarwhalTestResult *test_result,
                                char *error_message,
                                size_t message_size)
{
    push_data(&message_size, sizeof(message_size));
    push_data(error_message, message_size);
}

#undef push_data

/*
 * Set failing test result
 */

void narwhal_set_assertion_failure(NarwhalTestResult *test_result,
                                   char *failed_assertion,
                                   char *assertion_file,
                                   size_t assertion_line)
{
    if (failed_assertion != NULL)
    {
        size_t assertion_size = strlen(failed_assertion) + 1;
        test_result->failed_assertion = malloc(assertion_size);
        strncpy(test_result->failed_assertion, failed_assertion, assertion_size);
    }
    else
    {
        test_result->failed_assertion = NULL;
    }

    size_t filename_size = strlen(assertion_file) + 1;
    test_result->assertion_file = malloc(filename_size);
    strncpy(test_result->assertion_file, assertion_file, filename_size);

    test_result->assertion_line = assertion_line;
}

void narwhal_set_error_message(NarwhalTestResult *test_result,
                               char *error_message,
                               size_t message_size)
{
    test_result->success = false;
    test_result->error_message = malloc(message_size);
    strncpy(test_result->error_message, error_message, message_size);
}

/*
 * Create param snapshots
 */

void narwhal_test_param_snapshot(NarwhalTestParamSnapshot *param_snapshot,
                                 NarwhalTestParam *test_param)
{
    param_snapshot->param = test_param;
    param_snapshot->index = test_param->index;
}

NarwhalTestParamSnapshot *narwhal_new_test_param_snapshot(NarwhalTestParam *test_param)
{
    NarwhalTestParamSnapshot *param_snapshot = malloc(sizeof(NarwhalTestParamSnapshot));
    narwhal_test_param_snapshot(param_snapshot, test_param);

    return param_snapshot;
}

/*
 * Cleanup
 */

void narwhal_free_test_param_snapshot(NarwhalTestParamSnapshot *param_snapshot)
{
    free(param_snapshot);
}

void narwhal_free_test_result(NarwhalTestResult *test_result)
{
    while (test_result->param_snapshots->count > 0)
    {
        NarwhalTestParamSnapshot *param_snapshot =
            narwhal_collection_pop(test_result->param_snapshots);
        narwhal_free_test_param_snapshot(param_snapshot);
    }
    narwhal_free_collection(test_result->param_snapshots);

    free(test_result->output_buffer);
    free(test_result->failed_assertion);

    free(test_result->assertion_file);
    free(test_result->error_message);

    free(test_result->diff_original);
    free(test_result->diff_modified);

    free(test_result);
}
// #include "narwhal/diff/diff.h"
#ifndef NARWHAL_DIFF_H
#define NARWHAL_DIFF_H

#include <stdlib.h>

// #include "narwhal/types.h"


struct NarwhalDiffMatrix
{
    size_t rows;
    size_t columns;
    int *content;
};

enum NarwhalDiffChunkType
{
    NARWHAL_DIFF_CHUNK_TYPE_MATCHED,
    NARWHAL_DIFF_CHUNK_TYPE_ADDED,
    NARWHAL_DIFF_CHUNK_TYPE_REPLACED,
    NARWHAL_DIFF_CHUNK_TYPE_DELETED
};

struct NarwhalDiff
{
    size_t size;
    NarwhalDiffChunk *chunks;
};

struct NarwhalDiffChunk
{
    NarwhalDiffChunkType type;
    size_t original_start;
    size_t original_end;
    size_t modified_start;
    size_t modified_end;
};

NarwhalDiffMatrix *narwhal_new_diff_matrix(size_t rows, size_t columns);
NarwhalDiffMatrix *narwhal_new_diff_matrix_from_lengths(size_t original_length,
                                                        size_t modified_lengths);
void narwhal_diff_matrix_fill_from_strings(NarwhalDiffMatrix *diff_matrix,
                                           char *original,
                                           char *modified);
void narwhal_diff_matrix_fill_from_lines(NarwhalDiffMatrix *diff_matrix,
                                         char *original,
                                         char *modified);
NarwhalDiff narwhal_diff_matrix_get_diff(NarwhalDiffMatrix *diff_matrix);

size_t narwhal_diff_matrix_index(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column);
int narwhal_diff_matrix_get(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column);
void narwhal_diff_matrix_set(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column, int value);

NarwhalDiff narwhal_diff_strings_lengths(char *original,
                                         size_t original_length,
                                         char *modified,
                                         size_t modified_length);
NarwhalDiff narwhal_diff_strings(char *original, char *modified);
NarwhalDiff narwhal_diff_lines(char *original, char *modified);

void narwhal_free_diff_matrix(NarwhalDiffMatrix *diff_matrix);

#endif


#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// #include "narwhal/utils.h"


/*
 * Diff matrix initialization
 */

static void initialize_diff_matrix(NarwhalDiffMatrix *diff_matrix, size_t rows, size_t columns)
{
    diff_matrix->rows = rows;
    diff_matrix->columns = columns;
    diff_matrix->content = malloc(rows * columns * sizeof(int));
}

NarwhalDiffMatrix *narwhal_new_diff_matrix(size_t rows, size_t columns)
{
    NarwhalDiffMatrix *diff_matrix = malloc(sizeof(NarwhalDiffMatrix));
    initialize_diff_matrix(diff_matrix, rows, columns);

    return diff_matrix;
}

/*
 * Diff matrix operations
 */

NarwhalDiffMatrix *narwhal_new_diff_matrix_from_lengths(size_t original_length,
                                                        size_t modified_length)
{
    NarwhalDiffMatrix *diff_matrix =
        narwhal_new_diff_matrix(modified_length + 1, original_length + 1);

    for (size_t i = 0; i < diff_matrix->rows; i++)
    {
        narwhal_diff_matrix_set(diff_matrix, i, 0, i);
    }

    for (size_t j = 0; j < diff_matrix->columns; j++)
    {
        narwhal_diff_matrix_set(diff_matrix, 0, j, j);
    }

    return diff_matrix;
}

static void fill_different(NarwhalDiffMatrix *diff_matrix, size_t i, size_t j)
{
    narwhal_diff_matrix_set(
        diff_matrix,
        i,
        j,
        narwhal_min_int(narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1),
                        narwhal_min_int(narwhal_diff_matrix_get(diff_matrix, i - 1, j),
                                        narwhal_diff_matrix_get(diff_matrix, i, j - 1))) +
            1);
}

static void fill_equal(NarwhalDiffMatrix *diff_matrix, size_t i, size_t j)
{
    narwhal_diff_matrix_set(diff_matrix, i, j, narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1));
}

void narwhal_diff_matrix_fill_from_strings(NarwhalDiffMatrix *diff_matrix,
                                           char *original,
                                           char *modified)
{
    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            if (original[j - 1] == modified[i - 1])
            {
                fill_equal(diff_matrix, i, j);
            }
            else
            {
                fill_different(diff_matrix, i, j);
            }
        }
    }
}

void narwhal_diff_matrix_fill_from_lines(NarwhalDiffMatrix *diff_matrix,
                                         char *original,
                                         char *modified)
{
    char *modified_pos;
    char *modified_line = modified;

    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        modified_pos = narwhal_next_line(modified_line);
        size_t modified_line_length = modified_pos - modified_line;

        char *original_pos;
        char *original_line = original;

        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            original_pos = narwhal_next_line(original_line);
            size_t original_line_length = original_pos - original_line;

            if (original_line_length == modified_line_length &&
                strncmp(original_line, modified_line, original_line_length) == 0)
            {
                fill_equal(diff_matrix, i, j);
            }
            else
            {
                fill_different(diff_matrix, i, j);
            }

            original_line = original_pos + 1;
        }

        modified_line = modified_pos + 1;
    }
}

NarwhalDiff narwhal_diff_matrix_get_diff(NarwhalDiffMatrix *diff_matrix)
{
    if (diff_matrix->rows == 1 && diff_matrix->columns == 1)
    {
        NarwhalDiff diff = { .size = 0, .chunks = NULL };
        return diff;
    }

    size_t capacity = 32;
    size_t size = 0;
    NarwhalDiffChunk *backtrack = malloc(capacity * sizeof(NarwhalDiffChunk));

    size_t i = diff_matrix->rows - 1;
    size_t j = diff_matrix->columns - 1;

    while (i > 0 || j > 0)
    {
        if (size == capacity)
        {
            capacity *= 2;
            backtrack = realloc(backtrack, capacity * sizeof(NarwhalDiffChunk));
        }

        NarwhalDiffChunk *current_chunk = &backtrack[size];
        size++;

        int current = narwhal_diff_matrix_get(diff_matrix, i, j);

        if (i > 0 && j > 0 && current == narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1) + 1)
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_REPLACED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
        else if (j > 0 && current == narwhal_diff_matrix_get(diff_matrix, i, j - 1) + 1)
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_DELETED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i;
            current_chunk->modified_end = i;
            j--;
        }
        else if (i > 0 && current == narwhal_diff_matrix_get(diff_matrix, i - 1, j) + 1)
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_ADDED;
            current_chunk->original_start = j;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
        }
        else if (i > 0 && j > 0 && current == narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1))
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_MATCHED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
    }

    NarwhalDiff diff = { size, malloc(size * sizeof(NarwhalDiffChunk)) };

    ssize_t backtrack_index = size - 1;
    size_t chunk_index = 0;

    diff.chunks[chunk_index] = backtrack[backtrack_index];

    for (backtrack_index--; backtrack_index >= 0; backtrack_index--)
    {
        NarwhalDiffChunk *chunk = &backtrack[backtrack_index];
        NarwhalDiffChunk *previous_chunk = &diff.chunks[chunk_index];

        if (chunk->type == previous_chunk->type)
        {
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else if ((chunk->type == NARWHAL_DIFF_CHUNK_TYPE_REPLACED &&
                  previous_chunk->type != NARWHAL_DIFF_CHUNK_TYPE_MATCHED) ||
                 (chunk->type != NARWHAL_DIFF_CHUNK_TYPE_MATCHED &&
                  previous_chunk->type == NARWHAL_DIFF_CHUNK_TYPE_REPLACED))
        {
            previous_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_REPLACED;
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else
        {
            chunk_index++;
            diff.chunks[chunk_index] = *chunk;
        }
    }

    free(backtrack);

    diff.size = chunk_index + 1;
    diff.chunks = realloc(diff.chunks, diff.size * sizeof(NarwhalDiffChunk));

    return diff;
}

size_t narwhal_diff_matrix_index(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return row * diff_matrix->columns + column;
}

int narwhal_diff_matrix_get(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return diff_matrix->content[narwhal_diff_matrix_index(diff_matrix, row, column)];
}

void narwhal_diff_matrix_set(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column, int value)
{
    diff_matrix->content[narwhal_diff_matrix_index(diff_matrix, row, column)] = value;
}

/*
 * Higher-level wrappers
 */

NarwhalDiff narwhal_diff_strings_lengths(char *original,
                                         size_t original_length,
                                         char *modified,
                                         size_t modified_length)
{
    NarwhalDiffMatrix *diff_matrix =
        narwhal_new_diff_matrix_from_lengths(original_length, modified_length);

    narwhal_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    NarwhalDiff diff = narwhal_diff_matrix_get_diff(diff_matrix);

    narwhal_free_diff_matrix(diff_matrix);

    return diff;
}

NarwhalDiff narwhal_diff_strings(char *original, char *modified)
{
    return narwhal_diff_strings_lengths(original, strlen(original), modified, strlen(modified));
}

NarwhalDiff narwhal_diff_lines(char *original, char *modified)
{
    size_t original_length = narwhal_count_chars(original, '\n') + 1;
    size_t modified_length = narwhal_count_chars(modified, '\n') + 1;

    NarwhalDiffMatrix *diff_matrix =
        narwhal_new_diff_matrix_from_lengths(original_length, modified_length);

    narwhal_diff_matrix_fill_from_lines(diff_matrix, original, modified);

    NarwhalDiff diff = narwhal_diff_matrix_get_diff(diff_matrix);

    narwhal_free_diff_matrix(diff_matrix);

    return diff;
}

/*
 * Cleanup
 */

void narwhal_free_diff_matrix(NarwhalDiffMatrix *diff_matrix)
{
    free(diff_matrix->content);
    free(diff_matrix);
}
// #include "narwhal/param/param.h"


#include <stdlib.h>
#include <string.h>

// #include "narwhal/collection/collection.h"


/*
 * Currently accessible params
 */

NarwhalCollection *_narwhal_current_params = NULL;

/*
 * Test param initialization
 */

static void initialize_test_param(NarwhalTestParam *test_param,
                                  char *name,
                                  void *values,
                                  size_t count,
                                  NarwhalTest *test)
{
    test_param->name = name;
    test_param->index = 0;
    test_param->count = count;
    test_param->values = values;
    test_param->test = test;
}

NarwhalTestParam *narwhal_new_test_param(char *name, void *values, size_t count, NarwhalTest *test)
{
    NarwhalTestParam *test_param = malloc(sizeof(NarwhalTestParam));
    initialize_test_param(test_param, name, values, count, test);

    return test_param;
}

/*
 * Get param from param collection
 */

NarwhalTestParam *narwhal_get_test_param(NarwhalCollection *params, char *param_name)
{
    NarwhalTestParam *test_param;
    NARWHAL_EACH(test_param, params)
    {
        if (strcmp(test_param->name, param_name) == 0)
        {
            return test_param;
        }
    }
    return NULL;
}

/*
 * Cleanup
 */

void narwhal_free_test_param(NarwhalTestParam *test_param)
{
    free(test_param);
}
// #include "narwhal/group/group.h"
#ifndef NARWHAL_GROUP_H
#define NARWHAL_GROUP_H

#include <stdlib.h>

// #include "narwhal/types.h"

// #include "narwhal/unused_attribute.h"
#ifndef NARWHAL_UNUSED_ATTRIBUTE_H
#define NARWHAL_UNUSED_ATTRIBUTE_H

#ifdef __GNUC__
#define UNUSED __attribute__((unused))
#else
#define UNUSED
#endif

#endif


struct NarwhalTestGroup
{
    char *name;
    NarwhalTestGroup *group;
    NarwhalCollection *subgroups;
    NarwhalCollection *tests;
};

NarwhalTestGroup *narwhal_new_test_group(char *name,
                                         NarwhalGroupItemRegistration *group_items,
                                         size_t item_count);
void narwhal_register_subgroup(NarwhalTestGroup *test_group,
                               char *name,
                               NarwhalGroupItemRegistration *group_items,
                               size_t item_count);
void narwhal_register_test(NarwhalTestGroup *test_group,
                           char *name,
                           char *filename,
                           size_t line_number,
                           NarwhalTestFunction function,
                           NarwhalTestModifierRegistration *test_modifiers,
                           size_t modifier_count);

void narwhal_free_test_group(NarwhalTestGroup *test_group);

#define DECLARE_GROUP(group_name)                                            \
    extern NarwhalGroupItemRegistration _narwhal_group_items_##group_name[]; \
    void group_name(NarwhalTestGroup *test_group)

#define TEST_GROUP(group_name, ...)                                                 \
    DECLARE_GROUP(group_name);                                                      \
    NarwhalGroupItemRegistration _narwhal_group_items_##group_name[] = __VA_ARGS__; \
    void group_name(NarwhalTestGroup *test_group)                                   \
    {                                                                               \
        narwhal_register_subgroup(test_group,                                       \
                                  #group_name,                                      \
                                  _narwhal_group_items_##group_name,                \
                                  sizeof(_narwhal_group_items_##group_name) /       \
                                      sizeof(*_narwhal_group_items_##group_name));  \
    }

#endif


#include <stdlib.h>

// #include "narwhal/collection/collection.h"

// #include "narwhal/test/test.h"


/*
 * Group creation
 */

static void initialize_test_group(NarwhalTestGroup *test_group,
                                  char *name,
                                  NarwhalGroupItemRegistration *group_items,
                                  size_t item_count)
{
    test_group->name = name;
    test_group->group = NULL;
    test_group->subgroups = narwhal_empty_collection();
    test_group->tests = narwhal_empty_collection();

    for (size_t i = 0; i < item_count; i++)
    {
        NarwhalGroupItemRegistration registration = group_items[i];
        registration(test_group);
    }
}

NarwhalTestGroup *narwhal_new_test_group(char *name,
                                         NarwhalGroupItemRegistration *group_items,
                                         size_t item_count)
{
    NarwhalTestGroup *test_group = malloc(sizeof(NarwhalTestGroup));
    initialize_test_group(test_group, name, group_items, item_count);

    return test_group;
}

/*
 * Group items registration
 */

void narwhal_register_subgroup(NarwhalTestGroup *test_group,
                               char *name,
                               NarwhalGroupItemRegistration *group_items,
                               size_t item_count)
{
    NarwhalTestGroup *subgroup = narwhal_new_test_group(name, group_items, item_count);
    subgroup->group = test_group;

    narwhal_collection_append(test_group->subgroups, subgroup);
}

void narwhal_register_test(NarwhalTestGroup *test_group,
                           char *name,
                           char *filename,
                           size_t line_number,
                           NarwhalTestFunction function,
                           NarwhalTestModifierRegistration *test_modifiers,
                           size_t modifier_count)
{
    NarwhalTest *test =
        narwhal_new_test(name, filename, line_number, function, test_modifiers, modifier_count);
    test->group = test_group;

    narwhal_collection_append(test_group->tests, test);
}

/*
 * Cleanup
 */

void narwhal_free_test_group(NarwhalTestGroup *test_group)
{
    while (test_group->subgroups->count > 0)
    {
        NarwhalTestGroup *subgroup = narwhal_collection_pop(test_group->subgroups);
        narwhal_free_test_group(subgroup);
    }
    narwhal_free_collection(test_group->subgroups);

    while (test_group->tests->count > 0)
    {
        NarwhalTest *test = narwhal_collection_pop(test_group->tests);
        narwhal_free_test(test);
    }
    narwhal_free_collection(test_group->tests);

    free(test_group);
}
// #include "narwhal/fixture/fixture.h"


#include <stdlib.h>
#include <string.h>

// #include "narwhal/collection/collection.h"


/*
 * Currently accessible fixtures
 */

NarwhalCollection *_narwhal_current_fixtures = NULL;

/*
 * Test fixture initialization
 */

static void initialize_test_fixture(NarwhalTestFixture *test_fixture,
                                    char *name,
                                    size_t fixture_size,
                                    NarwhalTestFixtureSetup setup,
                                    NarwhalTest *test,
                                    NarwhalTestModifierRegistration *test_modifiers,
                                    size_t modifier_count)
{
    test_fixture->name = name;
    test_fixture->size = fixture_size;
    test_fixture->value = NULL;
    test_fixture->setup = setup;
    test_fixture->cleanup = NULL;
    test_fixture->test = test;
    test_fixture->accessible_fixtures = narwhal_empty_collection();
    test_fixture->accessible_params = narwhal_empty_collection();

    for (size_t i = 0; i < modifier_count; i++)
    {
        NarwhalTestModifierRegistration registration = test_modifiers[i];
        registration(test_fixture->test,
                     test_fixture->accessible_params,
                     test_fixture->accessible_fixtures);
    }
}

NarwhalTestFixture *narwhal_new_test_fixture(char *name,
                                             size_t fixture_size,
                                             NarwhalTestFixtureSetup setup,
                                             NarwhalTest *test,
                                             NarwhalTestModifierRegistration *test_modifiers,
                                             size_t modifier_count)
{
    NarwhalTestFixture *test_fixture = malloc(sizeof(NarwhalTestFixture));
    initialize_test_fixture(
        test_fixture, name, fixture_size, setup, test, test_modifiers, modifier_count);

    return test_fixture;
}

/*
 * Get fixture from fixture collection
 */

NarwhalTestFixture *narwhal_get_test_fixture(NarwhalCollection *fixtures, char *fixture_name)
{
    NarwhalTestFixture *test_fixture;
    NARWHAL_EACH(test_fixture, fixtures)
    {
        if (strcmp(test_fixture->name, fixture_name) == 0)
        {
            return test_fixture;
        }
    }
    return NULL;
}

/*
 * Cleanup
 */

void narwhal_free_test_fixture(NarwhalTestFixture *test_fixture)
{
    while (test_fixture->accessible_fixtures->count > 0)
    {
        narwhal_collection_pop(test_fixture->accessible_fixtures);
    }
    narwhal_free_collection(test_fixture->accessible_fixtures);

    while (test_fixture->accessible_params->count > 0)
    {
        narwhal_collection_pop(test_fixture->accessible_params);
    }
    narwhal_free_collection(test_fixture->accessible_params);

    free(test_fixture);
}
// #include "narwhal/output/output.h"
#ifndef NARWHAL_OUTPUT_H
#define NARWHAL_OUTPUT_H

#include <stdio.h>

// #include "narwhal/types.h"


void narwhal_output_string(FILE *stream, char *string, size_t line_number, char *indent);

void narwhal_output_session_init(NarwhalTestSession *test_session);
void narwhal_output_session_progress(NarwhalTestSession *test_session);
void narwhal_output_session_result(NarwhalTestSession *test_session);

#endif


#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

// #include "narwhal/collection/collection.h"

// #include "narwhal/diff/diff.h"

// #include "narwhal/group/group.h"

// #include "narwhal/output/ansi.h"
#ifndef NARWHAL_OUTPUT_ANSI_H
#define NARWHAL_OUTPUT_ANSI_H

#define INDENT "    "

#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"

#define ANSI_BOLD "\x1b[1m"
#define ANSI_RESET "\x1b[0m"

#define COLOR(color, ...) ANSI_RESET ANSI_COLOR_##color __VA_ARGS__ ANSI_RESET

#define BOLD(...) ANSI_RESET ANSI_BOLD __VA_ARGS__ ANSI_RESET

#define COLOR_BOLD(color, ...) ANSI_RESET ANSI_COLOR_##color ANSI_BOLD __VA_ARGS__ ANSI_RESET

#endif

// #include "narwhal/param/param.h"

// #include "narwhal/result/result.h"

// #include "narwhal/session/session.h"
#ifndef NARWHAL_SESSION_H
#define NARWHAL_SESSION_H

#include <sys/time.h>

// #include "narwhal/types.h"


struct NarwhalSessionOutputState
{
    int index;
    int length;
    char string[50];
};

struct NarwhalTestSession
{
    NarwhalCollection *results;
    NarwhalCollection *failures;
    struct timeval start_time;
    struct timeval end_time;
    NarwhalSessionOutputState output_state;
};

NarwhalTestSession *narwhal_new_test_session();

void narwhal_test_session_start(NarwhalTestSession *test_session);
void narwhal_test_session_end(NarwhalTestSession *test_session);

void narwhal_test_session_run_test(NarwhalTestSession *test_session, NarwhalTest *test);
void narwhal_test_session_run_parameterized_test(NarwhalTestSession *test_session,
                                                 NarwhalTest *test,
                                                 NarwhalCollectionItem *param_item);
void narwhal_test_session_run_test_group(NarwhalTestSession *test_session,
                                         NarwhalTestGroup *test_group);

void narwhal_free_test_session(NarwhalTestSession *test_session);

#endif

// #include "narwhal/test/test.h"

// #include "narwhal/utils.h"


/*
 * Formatting utilities
 */

static void full_test_name(NarwhalTest *test, char *full_name, size_t buffer_size)
{
    strncpy(full_name, test->name, buffer_size - 1);
    NarwhalTestGroup *parent_group = test->group;

    if (parent_group != NULL)
    {
        while (parent_group->group != NULL)
        {
            char current[buffer_size];
            memcpy(current, full_name, buffer_size);
            strncpy(full_name, parent_group->name, buffer_size - 1);
            strncat(full_name, "/", buffer_size - 1);
            strncat(full_name, current, buffer_size - 1);
            parent_group = parent_group->group;
        }
    }
}

static void format_param_snapshot(NarwhalTestParamSnapshot *param_snapshot,
                                  char *output_buffer,
                                  size_t buffer_size)
{
    snprintf(output_buffer,
             buffer_size,
             COLOR(MAGENTA, "%s") "[" COLOR(BLUE, "%zu") "]",
             param_snapshot->param->name,
             param_snapshot->index);
}

static void get_param_snapshots(NarwhalCollectionItem *snapshot_item,
                                char *output_buffer,
                                size_t buffer_size)
{
    NarwhalTestParamSnapshot *param_snapshot = snapshot_item->value;
    format_param_snapshot(param_snapshot, output_buffer, buffer_size);

    snapshot_item = snapshot_item->next;

    if (snapshot_item == NULL)
    {
        return;
    }

    param_snapshot = snapshot_item->value;

    while (snapshot_item->next != NULL)
    {
        char snapshot_string[buffer_size];
        format_param_snapshot(param_snapshot, snapshot_string, buffer_size);

        char new_value[buffer_size];
        snprintf(new_value, buffer_size, "%s, %s", output_buffer, snapshot_string);
        strncpy(output_buffer, new_value, buffer_size);

        snapshot_item = snapshot_item->next;
        param_snapshot = snapshot_item->value;
    }

    char snapshot_string[buffer_size];
    format_param_snapshot(param_snapshot, snapshot_string, buffer_size);

    strncat(output_buffer, " and ", buffer_size);
    strncat(output_buffer, snapshot_string, buffer_size);
}

static double elapsed_milliseconds(struct timeval start_time, struct timeval end_time)
{
    double milliseconds = (end_time.tv_sec) * 1000.0 + (end_time.tv_usec) / 1000.0;
    milliseconds -= (start_time.tv_sec) * 1000.0 + (start_time.tv_usec) / 1000.0;
    return milliseconds;
}

/*
 * Display result list
 */

static void display_test_result(NarwhalTestResult *test_result)
{
    printf(INDENT);

    char full_name[256];
    full_test_name(test_result->test, full_name, sizeof(full_name));

    if (test_result->success)
    {
        printf(COLOR_BOLD(GREEN, "PASS") " %s", full_name);
    }
    else
    {
        printf(COLOR_BOLD(RED, "FAIL") " " BOLD("%s"), full_name);
    }

    if (test_result->param_snapshots->count > 0)
    {
        char snapshot_string[256];
        get_param_snapshots(
            test_result->param_snapshots->first, snapshot_string, sizeof(snapshot_string));
        printf(" " COLOR_BOLD(BLUE, "with"));
        printf(" %s", snapshot_string);
    }

    printf(" (" COLOR_BOLD(YELLOW, "%.2fms") ")",
           elapsed_milliseconds(test_result->start_time, test_result->end_time));

    printf("\n");
}

static void display_results(NarwhalTestSession *test_session)
{
    printf("\nTest results:\n\n");

    NarwhalTestResult *test_result;
    NARWHAL_EACH(test_result, test_session->results) { display_test_result(test_result); }
}

/*
 * Display failures
 */

static void display_assertion(char *filename, size_t assertion_line)
{
    FILE *file = fopen(filename, "r");

    if (file == NULL)
    {
        return;
    }

    printf("\n");

    char line[256];
    size_t line_number = 0;

    size_t padding = 2;
    size_t before_assertion = assertion_line - padding;
    size_t after_assertion = assertion_line + padding;

    bool not_last_line = false;

    while (line_number < after_assertion &&
           (not_last_line = fgets(line, sizeof(line), file) != NULL))
    {
        line_number++;

        if (line_number < before_assertion || after_assertion < line_number)
        {
            continue;
        }

        if (line_number == assertion_line)
        {
            char line_prefix[64];
            snprintf(
                line_prefix, sizeof(line_prefix), "> " COLOR_BOLD(MAGENTA, "%ld"), line_number);
            printf("    %23s", line_prefix);
            printf(" |  " COLOR_BOLD(CYAN, "%s"), line);
        }
        else
        {
            printf("    " COLOR(MAGENTA, "%6zu"), line_number);
            printf(" |  %s", line);
        }
    }

    if (!not_last_line)
    {
        printf("\n");
    }

    fclose(file);
}

static char *display_inline_diff(NarwhalDiff *inline_diff,
                                 size_t lines,
                                 char *string,
                                 size_t *line_number,
                                 bool use_original)
{
    NarwhalDiffChunk *inline_chunk = &inline_diff->chunks[0];

    size_t line_index = 0;
    size_t index = 0;

    for (size_t i = 0; i < lines; i++)
    {
        char *next = narwhal_next_line(string);
        size_t line_length = next - string;

        char line_prefix[64];

        if (use_original)
        {
            snprintf(line_prefix,
                     sizeof(line_prefix),
                     COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"),
                     *line_number);
            printf("   %37s" COLOR(RED, " |  "), line_prefix);
        }
        else
        {
            snprintf(line_prefix,
                     sizeof(line_prefix),
                     COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"),
                     *line_number);
            printf("   %37s" COLOR(GREEN, " |  "), line_prefix);
        }

        while (index - line_index < line_length)
        {
            size_t chunk_end =
                use_original ? inline_chunk->original_end : inline_chunk->modified_end;

            size_t start = index - line_index;
            size_t end = narwhal_min_size_t(chunk_end - line_index, line_length);

            size_t characters = end - start;

            if (inline_chunk->type == NARWHAL_DIFF_CHUNK_TYPE_MATCHED)
            {
                printf("%.*s", (int)characters, string + index - line_index);
            }
            else if (characters > 0)
            {
                if (use_original)
                {
                    printf(COLOR_BOLD(RED, "%.*s"), (int)characters, string + index - line_index);
                }
                else
                {
                    printf(
                        COLOR_BOLD(GREEN, "%.*s"), (int)characters, string + index - line_index);
                }
            }

            index += characters;

            if (index >= chunk_end)
            {
                inline_chunk++;
            }
        }

        printf("\n");

        if (!use_original)
        {
            (*line_number)++;
        }

        string = next + 1;
        line_index += line_length + 1;
        index = line_index;
    }

    return string;
}

static void display_diff(char *original, char *modified)
{
    printf(INDENT INDENT "Diff:\n\n");

    NarwhalDiff diff = narwhal_diff_lines(original, modified);

    size_t line_number = 1;

    for (size_t chunk_index = 0; chunk_index < diff.size; chunk_index++)
    {
        NarwhalDiffChunk *chunk = &diff.chunks[chunk_index];

        size_t original_lines = chunk->original_end - chunk->original_start;
        size_t modified_lines = chunk->modified_end - chunk->modified_start;

        if (chunk->type == NARWHAL_DIFF_CHUNK_TYPE_MATCHED)
        {
            for (size_t i = 0; i < original_lines; i++)
            {
                char *original_next = narwhal_next_line(original);
                char *modified_next = narwhal_next_line(modified);

                if (original_lines < 7 || (i < 2 && chunk_index > 0) ||
                    (original_lines - i < 3 && chunk_index < diff.size - 1))
                {
                    printf(INDENT COLOR(MAGENTA, "%6zu"), line_number);
                    printf(" |  %.*s\n", (int)(original_next - original), original);
                }
                else if (i == 2)
                {
                    printf(INDENT INDENT "   :\n");
                }

                line_number++;
                original = original_next + 1;
                modified = modified_next + 1;
            }
        }
        else if (chunk->type == NARWHAL_DIFF_CHUNK_TYPE_REPLACED)
        {
            char *original_end = narwhal_next_lines(original, original_lines);
            char *modified_end = narwhal_next_lines(modified, modified_lines);

            size_t original_length = original_end - original;
            size_t modified_length = modified_end - modified;

            NarwhalDiff inline_diff =
                narwhal_diff_strings_lengths(original, original_length, modified, modified_length);

            original =
                display_inline_diff(&inline_diff, original_lines, original, &line_number, true);
            modified =
                display_inline_diff(&inline_diff, modified_lines, modified, &line_number, false);

            free(inline_diff.chunks);
        }
        else if (chunk->type == NARWHAL_DIFF_CHUNK_TYPE_DELETED)
        {
            for (size_t i = 0; i < original_lines; i++)
            {
                char *original_next = narwhal_next_line(original);

                char line_prefix[64];
                snprintf(line_prefix,
                         sizeof(line_prefix),
                         COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"),
                         line_number);

                printf("   %37s", line_prefix);
                printf(COLOR(RED, " |  ") COLOR_BOLD(RED, "%.*s\n"),
                       (int)(original_next - original),
                       original);

                original = original_next + 1;
            }
        }
        else if (chunk->type == NARWHAL_DIFF_CHUNK_TYPE_ADDED)
        {
            for (size_t i = 0; i < modified_lines; i++)
            {
                char *modified_next = narwhal_next_line(modified);

                char line_prefix[64];
                snprintf(line_prefix,
                         sizeof(line_prefix),
                         COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"),
                         line_number);

                printf("   %37s", line_prefix);
                printf(COLOR(GREEN, " |  ") COLOR_BOLD(GREEN, "%.*s\n"),
                       (int)(modified_next - modified),
                       modified);

                line_number++;
                modified = modified_next + 1;
            }
        }
    }

    free(diff.chunks);
}

static void display_output(char *output)
{
    printf(INDENT INDENT "Output:\n\n");

    narwhal_output_string(stdout, output, 1, INDENT);
}

static void display_failure(NarwhalTestResult *test_result)
{
    NarwhalTest *test = test_result->test;

    char full_name[256];
    full_test_name(test, full_name, sizeof(full_name));

    printf("\n" INDENT BOLD("%s"), full_name);

    if (test_result->param_snapshots->count > 0)
    {
        char snapshot_string[256];
        get_param_snapshots(
            test_result->param_snapshots->first, snapshot_string, sizeof(snapshot_string));
        printf(" " COLOR_BOLD(BLUE, "with"));
        printf(" %s", snapshot_string);
    }

    printf(" failed:\n");

    printf("\n" INDENT INDENT "Location: " COLOR(GREEN, "%s:%zu") "\n",
           test_result->assertion_file,
           test_result->assertion_line);
    printf(INDENT INDENT "Time:     " COLOR_BOLD(YELLOW, "%.2fms") "\n",
           elapsed_milliseconds(test_result->start_time, test_result->end_time));
    printf(INDENT INDENT "Error:    ");

    if (test_result->failed_assertion != NULL)
    {
        printf(COLOR_BOLD(RED, "Assertion"));

        if (narwhal_is_short_string(test_result->failed_assertion))
        {
            printf(" " COLOR_BOLD(CYAN, "%s"), test_result->failed_assertion);
        }

        printf(COLOR_BOLD(RED, " failed."));
        printf("\n" INDENT INDENT INDENT INDENT "  ");
    }

    bool has_diff = narwhal_test_result_has_diff(test_result);

    if (has_diff)
    {
        printf(COLOR_BOLD(RED, "%s"), "See diff for details.");
    }
    else if (strlen(test_result->error_message) > 0)
    {
        printf(COLOR_BOLD(RED, "%s"), test_result->error_message);
    }
    else
    {
        printf(COLOR_BOLD(RED, "%s"), "No details available.");
    }

    printf("\n");

    if (test_result->assertion_line != test->line_number ||
        strcmp(test_result->assertion_file, test->filename) != 0)
    {
        display_assertion(test_result->assertion_file, test_result->assertion_line);
    }

    if (has_diff)
    {
        printf("\n");
        display_diff(test_result->diff_original, test_result->diff_modified);
    }

    if (test_result->output_length > 0)
    {
        printf("\n");
        display_output(test_result->output_buffer);
    }
}

static void display_failing_tests(NarwhalTestSession *test_session)
{
    printf("\nFailing tests:\n");

    NarwhalTestResult *test_result;
    NARWHAL_EACH(test_result, test_session->failures) { display_failure(test_result); }
}

/*
 * Display session summary
 */

static void display_session_summary(NarwhalTestSession *test_session)
{
    printf("\nTests: ");

    if (test_session->failures->count > 0)
    {
        printf(COLOR_BOLD(RED, "%zu failed") ", ", test_session->failures->count);
    }
    printf(COLOR_BOLD(GREEN, "%zu passed") ", ",
           test_session->results->count - test_session->failures->count);
    printf("%zu total\n", test_session->results->count);

    printf("Time:  " COLOR_BOLD(YELLOW, "%.2fms") "\n",
           elapsed_milliseconds(test_session->start_time, test_session->end_time));
}

/*
 * Progress utils
 */

static void display_dot_string(NarwhalSessionOutputState *output_state)
{
    char string_label[64];

    snprintf(string_label,
             sizeof(string_label),
             COLOR(MAGENTA, "%d") " - " COLOR(MAGENTA, "%d"),
             output_state->index,
             output_state->index + output_state->length - 1);
    printf("%36s |  ", string_label);

    for (int i = 0; i < output_state->length; i++)
    {
        printf(output_state->string[i] == '.' ? "." : COLOR_BOLD(RED, "F"));
    }

    fflush(stdout);
}

/*
 * Public output functions
 */

void narwhal_output_string(FILE *stream, char *string, size_t line_number, char *indent)
{
    char *pos = strchr(string, '\n');
    char *line = string;

    while (pos != NULL)
    {
        fprintf(stream, "%s", indent);

        if (line_number > 0)
        {
            fprintf(stream, COLOR(MAGENTA, "%6zu"), line_number);
            fprintf(stream, " |  ");
        }

        fprintf(stream, "%.*s\n", (int)(pos - line), line);

        line = pos + 1;
        pos = strchr(line, '\n');

        if (line_number > 0)
        {
            line_number++;
        }
    }

    fprintf(stream, "%s", indent);

    if (line_number > 0)
    {
        fprintf(stream, COLOR(MAGENTA, "%6zu"), line_number);
        fprintf(stream, " |  ");
    }

    fprintf(stream, "%s\n", line);
}

void narwhal_output_session_init(NarwhalTestSession *test_session)
{
    printf("\nRunning tests...\n");

    NarwhalSessionOutputState *output_state = &test_session->output_state;

    output_state->length = sizeof(output_state->string);
    output_state->index = -output_state->length + 1;
}

void narwhal_output_session_progress(NarwhalTestSession *test_session)
{
    NarwhalSessionOutputState *output_state = &test_session->output_state;
    NarwhalTestResult *last_result = test_session->results->last->value;

    if (output_state->length < (int)sizeof(output_state->string))
    {
        output_state->string[output_state->length] = last_result->success ? '.' : 'F';
        output_state->length++;

        printf("\r");
        display_dot_string(&test_session->output_state);
    }
    else
    {
        output_state->index += sizeof(output_state->string);
        output_state->length = 1;

        output_state->string[0] = last_result->success ? '.' : 'F';

        printf("\n");
        display_dot_string(&test_session->output_state);
    }
}

void narwhal_output_session_result(NarwhalTestSession *test_session)
{
    if (test_session->results->count > 0)
    {
        printf("\n");
        display_results(test_session);
    }

    if (test_session->failures->count > 0)
    {
        display_failing_tests(test_session);
    }

    display_session_summary(test_session);
}
// #include "narwhal/session/session.h"


#include <stdlib.h>
#include <sys/time.h>

// #include "narwhal/collection/collection.h"

// #include "narwhal/group/group.h"

// #include "narwhal/output/output.h"

// #include "narwhal/param/param.h"

// #include "narwhal/result/result.h"

// #include "narwhal/test/test.h"


/*
 * Initialize test session
 */

static void initialize_test_session(NarwhalTestSession *test_session)
{
    test_session->results = narwhal_empty_collection();
    test_session->failures = narwhal_empty_collection();
}

NarwhalTestSession *narwhal_new_test_session()
{
    NarwhalTestSession *test_session = malloc(sizeof(NarwhalTestSession));
    initialize_test_session(test_session);

    return test_session;
}

/*
 * Test session hooks
 */

void narwhal_test_session_start(NarwhalTestSession *test_session)
{
    gettimeofday(&test_session->start_time, NULL);

    narwhal_output_session_init(test_session);
}

void narwhal_test_session_end(NarwhalTestSession *test_session)
{
    gettimeofday(&test_session->end_time, NULL);

    narwhal_output_session_result(test_session);
}

/*
 * Run test session
 */

static void register_result(NarwhalTestSession *test_session, NarwhalTestResult *test_result)
{
    narwhal_collection_append(test_session->results, test_result);

    if (!test_result->success)
    {
        narwhal_collection_append(test_session->failures, test_result);
    }
}

void narwhal_test_session_run_test(NarwhalTestSession *test_session, NarwhalTest *test)
{
    narwhal_run_test(test);
    register_result(test_session, test->result);

    narwhal_output_session_progress(test_session);
}

void narwhal_test_session_run_parameterized_test(NarwhalTestSession *test_session,
                                                 NarwhalTest *test,
                                                 NarwhalCollectionItem *param_item)
{
    if (param_item == NULL)
    {
        narwhal_test_session_run_test(test_session, test);
        return;
    }

    NarwhalTestParam *param = param_item->value;

    for (param->index = 0; param->index < param->count; param->index++)
    {
        narwhal_test_session_run_parameterized_test(test_session, test, param_item->next);
    }
}

void narwhal_test_session_run_test_group(NarwhalTestSession *test_session,
                                         NarwhalTestGroup *test_group)
{
    NarwhalTestGroup *subgroup;
    NARWHAL_EACH(subgroup, test_group->subgroups)
    {
        narwhal_test_session_run_test_group(test_session, subgroup);
    }

    NarwhalTest *test;
    NARWHAL_EACH(test, test_group->tests)
    {
        narwhal_test_session_run_parameterized_test(test_session, test, test->params->first);
    }
}

/*
 * Cleanup
 */

void narwhal_free_test_session(NarwhalTestSession *test_session)
{
    while (test_session->results->count > 0)
    {
        NarwhalTestResult *test_result = narwhal_collection_pop(test_session->results);
        narwhal_free_test_result(test_result);
    }
    narwhal_free_collection(test_session->results);

    while (test_session->failures->count > 0)
    {
        narwhal_collection_pop(test_session->failures);
    }
    narwhal_free_collection(test_session->failures);

    free(test_session);
}
// #include "narwhal/collection/collection.h"


#include <stdlib.h>

/*
 * Collection initialization
 */

static void initialize_collection(NarwhalCollection *collection)
{
    collection->count = 0;
    collection->first = NULL;
    collection->last = NULL;
}

static void collection_item(NarwhalCollectionItem *item, void *value)
{
    item->value = value;
    item->previous = NULL;
    item->next = NULL;
}

static NarwhalCollectionItem *narwhal_new_collection_item(void *value)
{
    NarwhalCollectionItem *item = malloc(sizeof(NarwhalCollectionItem));
    collection_item(item, value);

    return item;
}

NarwhalCollection *narwhal_empty_collection()
{
    NarwhalCollection *collection = malloc(sizeof(NarwhalCollection));
    initialize_collection(collection);

    return collection;
}

/*
 * Collection operations
 */

void narwhal_collection_append(NarwhalCollection *collection, void *value)
{
    NarwhalCollectionItem *new_item = narwhal_new_collection_item(value);
    new_item->previous = collection->last;
    collection->last = new_item;
    collection->count++;

    if (collection->count > 1)
    {
        new_item->previous->next = new_item;
    }
    else
    {
        collection->first = new_item;
    }
}

void *narwhal_collection_pop(NarwhalCollection *collection)
{
    if (collection->count == 0)
    {
        return NULL;
    }

    NarwhalCollectionItem *last_item = collection->last;
    collection->last = last_item->previous;
    collection->count--;

    void *value = last_item->value;
    free(last_item);

    if (collection->count > 0)
    {
        collection->last->next = NULL;
    }
    else
    {
        collection->first = NULL;
    }

    return value;
}

/*
 * Cleanup
 */

void narwhal_free_collection(NarwhalCollection *collection)
{
    free(collection);
}
// #include "narwhal/narwhal.h"
#ifndef NARWHAL_H
#define NARWHAL_H

// #include "narwhal/assertion/assertion.h"
#ifndef NARWHAL_ASSERTION_H
#define NARWHAL_ASSERTION_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "narwhal/types.h"


void narwhal_fail_test(NarwhalTest *test, char *format, ...);
bool narwhal_check_assertion(NarwhalTest *test,
                             bool assertion_success,
                             char *assertion,
                             char *assertion_file,
                             size_t assertion_line);

bool narwhal_check_string_equal(char *actual, char *expected);
bool narwhal_check_substring(char *string, char *substring);

char *narwhal_assertion_process_string(char *string);

#define _NARWHAL_TEST_FAILURE(...)                             \
    ({                                                         \
        narwhal_fail_test(_narwhal_current_test, __VA_ARGS__); \
        return;                                                \
    })

#define FAIL(...)                                                         \
    if (({                                                                \
            narwhal_pipe_assertion_failure(                               \
                _narwhal_current_test->result, NULL, __FILE__, __LINE__); \
            true;                                                         \
        }))                                                               \
    _NARWHAL_TEST_FAILURE("" __VA_ARGS__)

#define ASSERT(assertion, ...)                                                   \
    if (narwhal_check_assertion(                                                 \
            _narwhal_current_test, (assertion), #assertion, __FILE__, __LINE__)) \
    _NARWHAL_TEST_FAILURE("" __VA_ARGS__)

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

#endif

// #include "narwhal/collection/collection.h"

// #include "narwhal/diff/diff.h"

// #include "narwhal/fixture/fixture.h"

// #include "narwhal/group/group.h"

// #include "narwhal/output/output.h"

// #include "narwhal/param/param.h"

// #include "narwhal/result/result.h"

// #include "narwhal/session/session.h"

// #include "narwhal/test/test.h"

// #include "narwhal/test_utils/test_utils.h"

// #include "narwhal/types.h"


int narwhal_run_root_group(NarwhalGroupItemRegistration *root_items, size_t item_count);

#define RUN_TESTS(...)                                                                      \
    ({                                                                                      \
        NarwhalGroupItemRegistration _narwhal_root_items[] = { __VA_ARGS__ };               \
        narwhal_run_root_group(_narwhal_root_items,                                         \
                               sizeof(_narwhal_root_items) / sizeof(*_narwhal_root_items)); \
    })

#endif


#include <stdlib.h>

/*
 * Main run function
 */

int narwhal_run_root_group(NarwhalGroupItemRegistration *root_items, size_t item_count)
{
    NarwhalTestGroup *root_group = narwhal_new_test_group("root", root_items, item_count);
    NarwhalTestSession *test_session = narwhal_new_test_session();

    narwhal_test_session_start(test_session);
    narwhal_test_session_run_test_group(test_session, root_group);
    narwhal_test_session_end(test_session);

    int status = test_session->failures->count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

    narwhal_free_test_group(root_group);
    narwhal_free_test_session(test_session);

    return status;
}
// #include "narwhal/test_utils/test_utils.h"


#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// #include "narwhal/test/test.h"

// #include "narwhal/utils.h"


NarwhalOutputCapture _narwhal_default_output_capture = { .initialization_phase = true,
                                                         .stdout_backup = -1,
                                                         .stderr_backup = -1,
                                                         .pipe = { -1, -1 },
                                                         .parent = NULL };

/*
 * Initialize capture
 */

static void initialize_output_capture(NarwhalOutputCapture *capture)
{
    if (pipe(capture->pipe) == -1)
    {
        fprintf(stderr, "Failed to create capture pipe.\n");
        exit(EXIT_FAILURE);
    }

    capture->parent = _narwhal_current_test->output_capture;
    _narwhal_current_test->output_capture = capture;

    capture->stdout_backup = dup(STDOUT_FILENO);
    capture->stderr_backup = dup(STDERR_FILENO);

    while (dup2(capture->pipe[1], STDOUT_FILENO) == -1 && errno == EINTR)
        ;
    while (dup2(capture->pipe[1], STDERR_FILENO) == -1 && errno == EINTR)
        ;
}

/*
 * Finalize capture
 */

static void finalize_output_capture(NarwhalOutputCapture *capture, char **output_buffer)
{
    bool terminator_written = write(capture->pipe[1], "", 1) == 1;

    close(capture->pipe[1]);

    dup2(capture->stdout_backup, STDOUT_FILENO);
    dup2(capture->stderr_backup, STDERR_FILENO);

    if (terminator_written)
    {
        FILE *stream = fdopen(capture->pipe[0], "r");

        ssize_t output_length = narwhal_util_read_stream(stream, output_buffer) - 1;

        if (write(STDOUT_FILENO, *output_buffer, output_length) != output_length)
        {
            fprintf(stderr, "Failed to write captured output to stdout");
        }

        fclose(stream);
    }
    else
    {
        fprintf(stderr, "Failed to write to capture pipe.\n");
    }

    _narwhal_current_test->output_capture = capture->parent;

    close(capture->pipe[0]);
}

/*
 * Main capture function
 */

bool narwhal_capture_output(NarwhalOutputCapture *capture, char **output_buffer)
{
    fflush(stdout);
    fflush(stderr);

    if (capture->initialization_phase)
    {
        initialize_output_capture(capture);
        capture->initialization_phase = false;

        return true;
    }
    else
    {
        finalize_output_capture(capture, output_buffer);
        auto_free(*output_buffer);

        return false;
    }
}
// #include "narwhal/assertion/assertion.h"


#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #include "narwhal/output/ansi.h"

// #include "narwhal/output/output.h"

// #include "narwhal/result/result.h"

// #include "narwhal/test/test.h"

// #include "narwhal/utils.h"


void narwhal_fail_test(NarwhalTest *test, char *format, ...)
{
    size_t buffer_size;
    char *message;
    FILE *stream = open_memstream(&message, &buffer_size);

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fclose(stream);

    narwhal_pipe_error_message(test->result, message, buffer_size + 1);

    free(message);
}

bool narwhal_check_assertion(NarwhalTest *test,
                             bool assertion_success,
                             char *assertion,
                             char *assertion_file,
                             size_t assertion_line)
{
    if (assertion_success)
    {
        return false;
    }

    narwhal_pipe_assertion_failure(test->result, assertion, assertion_file, assertion_line);
    return true;
}

bool narwhal_check_string_equal(char *actual, char *expected)
{
    if (strcmp(actual, expected) == 0)
    {
        return true;
    }

    NarwhalTestResult *test_result = _narwhal_current_test->result;
    test_result->diff_original = expected;
    test_result->diff_original_size = strlen(expected) + 1;
    test_result->diff_modified = actual;
    test_result->diff_modified_size = strlen(actual) + 1;

    return false;
}

bool narwhal_check_substring(char *string, char *substring)
{
    return string != NULL && substring != NULL && strstr(string, substring) != NULL;
}

char *narwhal_assertion_process_string(char *string)
{
    if (narwhal_is_short_string(string))
    {
        return string;
    }

    size_t buffer_size;
    char *buffer;
    FILE *stream = open_memstream(&buffer, &buffer_size);

    fprintf(stream, "\n");
    narwhal_output_string(stream, string, 0, "  " INDENT INDENT INDENT INDENT INDENT ANSI_RESET);
    fprintf(stream, ANSI_COLOR_RED ANSI_BOLD "  " INDENT INDENT INDENT INDENT);

    fclose(stream);

    auto_free(buffer);

    return buffer;
}
// #include "narwhal/utils.h"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

size_t narwhal_util_read_stream(FILE *stream, char **output_buffer)
{
    char buffer[256];
    size_t output_length = 0;

    ssize_t read_count = fread(buffer, 1, sizeof(buffer) - 1, stream);
    buffer[read_count] = '\0';

    if (read_count > 0)
    {
        *output_buffer = malloc(read_count + 1);
        memcpy(*output_buffer, buffer, read_count + 1);
    }

    output_length = read_count;

    while (read_count + 1 == sizeof(buffer))
    {
        read_count = fread(buffer, 1, sizeof(buffer) - 1, stream);
        buffer[read_count] = '\0';
        output_length += read_count;

        *output_buffer = realloc(*output_buffer, output_length + 1);
        memcpy(*output_buffer + output_length - read_count, buffer, read_count + 1);
    }

    return output_length;
}

bool narwhal_is_short_string(char *string)
{
    return strlen(string) < 64 && strchr(string, '\n') == NULL;
}

int narwhal_min_int(int a, int b)
{
    return a < b ? a : b;
}

size_t narwhal_min_size_t(size_t a, size_t b)
{
    return a < b ? a : b;
}

size_t narwhal_count_chars(char *string, char chr)
{
    size_t count = 0;

    for (size_t i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == chr)
        {
            count++;
        }
    }

    return count;
}

char *narwhal_next_line(char *string)
{
    char *next_line = strchr(string, '\n');

    if (next_line != NULL)
    {
        return next_line;
    }
    else
    {
        return string + strlen(string);
    }
}

char *narwhal_next_lines(char *string, size_t lines)
{
    char *next_line = string;

    for (size_t i = 0; i < lines; i++)
    {
        next_line = narwhal_next_line(next_line) + 1;
    }

    return next_line;
}
