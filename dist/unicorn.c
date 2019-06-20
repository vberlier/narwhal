/*
Unicorn v0.1.1 (https://github.com/vberlier/unicorn)
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

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

// #include "unicorn/collection/collection.h"
#ifndef UNICORN_COLLECTION_H
#define UNICORN_COLLECTION_H


#include <stdlib.h>
#include <stdbool.h>

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

// #include "unicorn/utils.h"
#ifndef UNICORN_UTILS_H
#define UNICORN_UTILS_H


#include <stdio.h>
#include <stdbool.h>


size_t unicorn_util_read_stream(FILE *stream, char **buffer);
bool unicorn_is_short_string(char *string);
int unicorn_min_int(int a, int b);
size_t unicorn_min_size_t(size_t a, size_t b);
size_t unicorn_count_chars(char *string, char chr);
char *unicorn_next_line(char *string);
char *unicorn_next_lines(char *string, size_t lines);


#endif



/*
 * Current test
 */

UnicornTest *_unicorn_current_test = NULL;


/*
 * Test creation
 */

static void initialize_test(UnicornTest *test, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    test->name = name;
    test->filename = filename;
    test->line_number = line_number;
    test->group = NULL;
    test->function = function;
    test->resources = unicorn_empty_collection();
    test->fixtures = unicorn_empty_collection();
    test->params = unicorn_empty_collection();
    test->accessible_fixtures = unicorn_empty_collection();
    test->accessible_params = unicorn_empty_collection();
    test->result = NULL;
    test->output_capture = NULL;

    for (size_t i = 0; i < modifier_count; i++)
    {
        UnicornTestModifierRegistration registration = test_modifiers[i];
        registration(test, test->accessible_params, test->accessible_fixtures);
    }
}

UnicornTest *unicorn_new_test(char *name, char *filename, size_t line_number, UnicornTestFunction function, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    UnicornTest *test = malloc(sizeof (UnicornTest));
    initialize_test(test, name, filename, line_number, function, test_modifiers, modifier_count);

    return test;
}


/*
 * Test resources
 */

void unicorn_free_after_test(UnicornTest *test, void *resource)
{
    unicorn_collection_append(test->resources, resource);
}

void auto_free(void *resource)
{
    unicorn_free_after_test(_unicorn_current_test, resource);
}

void *unicorn_test_resource(UnicornTest *test, size_t size)
{
    void *resource = malloc(size);
    unicorn_free_after_test(test, resource);
    return resource;
}

void *test_resource(size_t size)
{
    return unicorn_test_resource(_unicorn_current_test, size);
}

void unicorn_free_test_resources(UnicornTest *test)
{
    while (test->resources->count > 0)
    {
        void *test_resource = unicorn_collection_pop(test->resources);
        free(test_resource);
    }
}


/*
 * Report test data
 */

#define pull_data(value, size) if (read(test_result->pipe[0], (value), (size)) != (ssize_t)(size)) \
    fprintf(stderr, "%s:%d: Failed to read from result pipe.\n", __FILE__, __LINE__)

static void test_error(UnicornTestResult *test_result, char *message, size_t message_size)
{
    unicorn_set_assertion_failure(test_result, NULL, test_result->test->filename, test_result->test->line_number);
    unicorn_set_error_message(test_result, message, message_size);
}

static void report_success(UnicornTestResult *test_result)
{
    bool test_success;
    ssize_t bytes_read = read(test_result->pipe[0], &test_success, sizeof (bool));

    if (bytes_read != sizeof (bool))
    {
        char message[] = "Test process exited unexpectedly.";
        test_error(test_result, message, sizeof (message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    pull_data(&test_result->start_time, sizeof (struct timeval));
    pull_data(&test_result->end_time, sizeof (struct timeval));
}

static void report_failure(UnicornTestResult *test_result)
{
    bool test_success;
    ssize_t bytes_read = read(test_result->pipe[0], &test_success, sizeof (bool));

    if (bytes_read != sizeof (bool))
    {
        char message[] = "Test process exited unexpectedly.";
        test_error(test_result, message, sizeof (message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    if (test_success)
    {
        pull_data(&test_result->start_time, sizeof (struct timeval));
        pull_data(&test_result->end_time, sizeof (struct timeval));

        char message[] = "Test process exited with non-zero return code.";
        test_error(test_result, message, sizeof (message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    size_t assertion_size;
    pull_data(&assertion_size, sizeof (size_t));

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
    pull_data(&filename_size, sizeof (size_t));

    test_result->assertion_file = malloc(filename_size);
    pull_data(test_result->assertion_file, filename_size);

    pull_data(&test_result->assertion_line, sizeof (size_t));

    bool has_diff;
    pull_data(&has_diff, sizeof (has_diff));

    if (has_diff)
    {
        pull_data(&test_result->diff_original_size, sizeof (test_result->diff_original_size));
        test_result->diff_original = malloc(test_result->diff_original_size);
        pull_data(test_result->diff_original, test_result->diff_original_size);

        pull_data(&test_result->diff_modified_size, sizeof (test_result->diff_modified_size));
        test_result->diff_modified = malloc(test_result->diff_modified_size);
        pull_data(test_result->diff_modified, test_result->diff_modified_size);
    }

    size_t message_size;
    pull_data(&message_size, sizeof (size_t));

    test_result->error_message = malloc(message_size);
    pull_data(test_result->error_message, message_size);

    pull_data(&test_result->start_time, sizeof (struct timeval));
    pull_data(&test_result->end_time, sizeof (struct timeval));
}

static void report_output(UnicornTestResult *test_result)
{
    FILE *stream = fdopen(test_result->output_pipe[0], "r");

    test_result->output_length = unicorn_util_read_stream(stream, &test_result->output_buffer);

    fclose(stream);
}

#undef pull_data


/*
 * Run test
 */

static void setup_test_result(UnicornTest *test)
{
    test->result = unicorn_new_test_result();
    test->result->test = test;

    UnicornTestParam *test_param;
    UNICORN_EACH(test_param, test->params)
    {
        UnicornTestParamSnapshot *param_snapshot = unicorn_new_test_param_snapshot(test_param);
        unicorn_collection_append(test->result->param_snapshots, param_snapshot);
    }
}

static int test_start(UnicornTest *test)
{
    bool test_success = test->result->success;

    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        test_fixture->cleanup = NULL;
        test_fixture->value = unicorn_test_resource(test, test_fixture->size);

        _unicorn_current_test = test;
        _unicorn_current_params = test_fixture->accessible_params;
        _unicorn_current_fixtures = test_fixture->accessible_fixtures;

        test_fixture->setup(test_fixture->value, test_fixture);

        _unicorn_current_test = NULL;
        _unicorn_current_params = NULL;
        _unicorn_current_fixtures = NULL;

        fflush(stdout);
        fflush(stderr);

        if (test->result->success != test_success)
        {
            break;
        }
    }

    return test->result->success == test_success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int test_end(UnicornTest *test)
{
    bool test_success = test->result->success;

    UnicornTestFixture *test_fixture;
    UNICORN_REVERSED(test_fixture, test->fixtures)
    {
        if (test_fixture->cleanup != NULL)
        {
            _unicorn_current_test = test;
            _unicorn_current_params = test_fixture->accessible_params;
            _unicorn_current_fixtures = test_fixture->accessible_fixtures;

            test_fixture->cleanup(test_fixture->value, test_fixture);

            _unicorn_current_test = NULL;
            _unicorn_current_params = NULL;
            _unicorn_current_fixtures = NULL;

            fflush(stdout);
            fflush(stderr);

            if (test->result->success != test_success)
            {
                break;
            }
        }
    }

    unicorn_free_test_resources(test);

    return test->result->success == test_success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static int execute_test_function(UnicornTest *test)
{
    struct timeval start_time;
    struct timeval end_time;

    gettimeofday(&start_time, NULL);

    if (test_start(test) == EXIT_FAILURE)
    {
        unicorn_pipe_test_info(test->result, start_time, start_time);
        return EXIT_FAILURE;
    }

    _unicorn_current_test = test;
    _unicorn_current_params = test->accessible_params;
    _unicorn_current_fixtures = test->accessible_fixtures;

    gettimeofday(&start_time, NULL);

    test->function();

    gettimeofday(&end_time, NULL);

    _unicorn_current_test = NULL;
    _unicorn_current_params = NULL;
    _unicorn_current_fixtures = NULL;

    fflush(stdout);
    fflush(stderr);

    if (test->result->success)
    {
        test_end(test);
        unicorn_pipe_test_info(test->result, start_time, end_time);
    }
    else
    {
        unicorn_pipe_test_info(test->result, start_time, end_time);
        test_end(test);
    }

    return test->result->success ? EXIT_SUCCESS : EXIT_FAILURE;
}

void unicorn_run_test(UnicornTest *test)
{
    setup_test_result(test);

    UnicornTestResult *test_result = test->result;

    if (pipe(test_result->pipe) == -1)
    {
        char message[] = "Couldn't create the test pipe.";
        test_error(test_result, message, sizeof (message));
        return;
    }

    if (pipe(test_result->output_pipe) == -1)
    {
        char message[] = "Couldn't create the output pipe.";
        test_error(test_result, message, sizeof (message));

        close(test_result->pipe[0]);
        close(test_result->pipe[1]);

        return;
    }

    pid_t test_pid = fork();

    if (test_pid == -1)
    {
        char message[] = "Couldn't create the test child process.";
        test_error(test_result, message, sizeof (message));

        close(test_result->pipe[0]);
        close(test_result->pipe[1]);
        close(test_result->output_pipe[0]);
        close(test_result->output_pipe[1]);

        return;
    }
    else if (test_pid == 0)
    {
        close(test_result->pipe[0]);

        while (dup2(test_result->output_pipe[1], STDOUT_FILENO) == -1 && errno == EINTR);
        while (dup2(test_result->output_pipe[1], STDERR_FILENO) == -1 && errno == EINTR);
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

void unicorn_register_test_fixture(UnicornTest *test, UnicornCollection *access_collection, char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    UnicornTestFixture *test_fixture = unicorn_get_test_fixture(test->fixtures, name);

    if (test_fixture == NULL)
    {
        test_fixture = unicorn_new_test_fixture(name, fixture_size, setup, test, test_modifiers, modifier_count);

        unicorn_collection_append(test->fixtures, test_fixture);
        unicorn_collection_append(access_collection, test_fixture);
    }
    else if (unicorn_get_test_fixture(access_collection, name) == NULL)
    {
        unicorn_collection_append(access_collection, test_fixture);
    }
}

void unicorn_register_test_param(UnicornTest *test, UnicornCollection *access_collection, char *name, void *values, size_t count)
{
    UnicornTestParam *test_param = unicorn_get_test_param(test->params, name);

    if (test_param == NULL)
    {
        test_param = unicorn_new_test_param(name, values, count, test);

        unicorn_collection_append(test->params, test_param);
        unicorn_collection_append(access_collection, test_param);
    }
    else if (unicorn_get_test_param(access_collection, name) == NULL)
    {
        unicorn_collection_append(access_collection, test_param);
    }
}


/*
 * Cleanup
 */

void unicorn_free_test(UnicornTest *test)
{
    while (test->accessible_fixtures->count > 0)
    {
        unicorn_collection_pop(test->accessible_fixtures);
    }
    unicorn_free_collection(test->accessible_fixtures);

    while (test->accessible_params->count > 0)
    {
        unicorn_collection_pop(test->accessible_params);
    }
    unicorn_free_collection(test->accessible_params);

    while (test->fixtures->count > 0)
    {
        UnicornTestFixture *test_fixture = unicorn_collection_pop(test->fixtures);
        unicorn_free_test_fixture(test_fixture);
    }
    unicorn_free_collection(test->fixtures);

    while (test->params->count > 0)
    {
        UnicornTestParam *test_param = unicorn_collection_pop(test->params);
        unicorn_free_test_param(test_param);
    }
    unicorn_free_collection(test->params);

    unicorn_free_collection(test->resources);

    free(test);
}
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

// #include "unicorn/collection/collection.h"

// #include "unicorn/param/param.h"

// #include "unicorn/result/result.h"

// #include "unicorn/test/test.h"

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



/*
 * Test result initialization
 */

static void initialize_test_result(UnicornTestResult *test_result)
{
    test_result->success = true;
    test_result->failed_assertion = NULL;
    test_result->error_message = NULL;
    test_result->assertion_file = NULL;
    test_result->assertion_line = 0;
    test_result->test = NULL;
    test_result->param_snapshots = unicorn_empty_collection();
    test_result->output_buffer = NULL;
    test_result->output_length = 0;
    test_result->diff_original = NULL;
    test_result->diff_original_size = -1;
    test_result->diff_modified = NULL;
    test_result->diff_modified_size = -1;
}

UnicornTestResult *unicorn_new_test_result()
{
    UnicornTestResult *test_result = malloc(sizeof (UnicornTestResult));
    initialize_test_result(test_result);

    return test_result;
}


/*
 * Util
 */

bool unicorn_test_result_has_diff(UnicornTestResult *test_result)
{
    return test_result->diff_original != NULL && test_result->diff_original_size > 0 && test_result->diff_modified != NULL && test_result->diff_modified_size > 0;
}


/*
 * Pipe test result data
 */

#define push_data(value, size) if (write(test_result->pipe[1], (value), (size)) != (ssize_t)(size)) \
    fprintf(stderr, "Failed to write to result pipe.\n")

void unicorn_pipe_test_info(UnicornTestResult *test_result, struct timeval start_time, struct timeval end_time)
{
    if (test_result->success)
    {
        push_data(&test_result->success, sizeof (bool));
    }

    push_data(&start_time, sizeof (start_time));
    push_data(&end_time, sizeof (end_time));
}

void unicorn_pipe_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, char *assertion_file, size_t assertion_line)
{
    while (test_result->test->output_capture != NULL)
    {
        char *output_buffer = NULL;
        while (unicorn_capture_output(test_result->test->output_capture, &output_buffer));
    }

    test_result->success = false;
    push_data(&test_result->success, sizeof (bool));

    size_t assertion_size = failed_assertion != NULL ? strlen(failed_assertion) + 1 : 0;
    push_data(&assertion_size, sizeof (assertion_size));
    push_data(failed_assertion, assertion_size);

    size_t filename_size = strlen(assertion_file) + 1;
    push_data(&filename_size, sizeof (filename_size));
    push_data(assertion_file, filename_size);

    push_data(&assertion_line, sizeof (assertion_line));

    bool has_diff = false;

    if (unicorn_test_result_has_diff(test_result))
    {
        has_diff = true;
        push_data(&has_diff, sizeof (has_diff));

        push_data(&test_result->diff_original_size, sizeof (test_result->diff_original_size));
        push_data(test_result->diff_original, test_result->diff_original_size);

        push_data(&test_result->diff_modified_size, sizeof (test_result->diff_modified_size));
        push_data(test_result->diff_modified, test_result->diff_modified_size);
    }
    else
    {
        push_data(&has_diff, sizeof (has_diff));
    }

}

void unicorn_pipe_error_message(UnicornTestResult *test_result, char *error_message, size_t message_size)
{
    push_data(&message_size, sizeof (message_size));
    push_data(error_message, message_size);
}

#undef push_data


/*
 * Set failing test result
 */

void unicorn_set_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, char *assertion_file, size_t assertion_line)
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

void unicorn_set_error_message(UnicornTestResult *test_result, char *error_message, size_t message_size)
{
    test_result->success = false;
    test_result->error_message = malloc(message_size);
    strncpy(test_result->error_message, error_message, message_size);
}


/*
 * Create param snapshots
 */

void unicorn_test_param_snapshot(UnicornTestParamSnapshot *param_snapshot, UnicornTestParam *test_param)
{
    param_snapshot->param = test_param;
    param_snapshot->index = test_param->index;
}

UnicornTestParamSnapshot *unicorn_new_test_param_snapshot(UnicornTestParam *test_param)
{
    UnicornTestParamSnapshot *param_snapshot = malloc(sizeof (UnicornTestParamSnapshot));
    unicorn_test_param_snapshot(param_snapshot, test_param);

    return param_snapshot;
}


/*
 * Cleanup
 */

void unicorn_free_test_param_snapshot(UnicornTestParamSnapshot *param_snapshot)
{
    free(param_snapshot);
}

void unicorn_free_test_result(UnicornTestResult *test_result)
{
    while (test_result->param_snapshots->count > 0)
    {
        UnicornTestParamSnapshot *param_snapshot = unicorn_collection_pop(test_result->param_snapshots);
        unicorn_free_test_param_snapshot(param_snapshot);
    }
    unicorn_free_collection(test_result->param_snapshots);

    free(test_result->output_buffer);
    free(test_result->failed_assertion);

    free(test_result->assertion_file);
    free(test_result->error_message);

    free(test_result->diff_original);
    free(test_result->diff_modified);

    free(test_result);
}
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

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

// #include "unicorn/utils.h"



/*
 * Diff matrix initialization
 */

static void initialize_diff_matrix(UnicornDiffMatrix *diff_matrix, size_t rows, size_t columns)
{
    diff_matrix->rows = rows;
    diff_matrix->columns = columns;
    diff_matrix->content = malloc(rows * columns * sizeof (int));
}

UnicornDiffMatrix *unicorn_new_diff_matrix(size_t rows, size_t columns)
{
    UnicornDiffMatrix *diff_matrix = malloc(sizeof (UnicornDiffMatrix));
    initialize_diff_matrix(diff_matrix, rows, columns);

    return diff_matrix;
}


/*
 * Diff matrix operations
 */

UnicornDiffMatrix *unicorn_new_diff_matrix_from_lengths(size_t original_length, size_t modified_length)
{
    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix(modified_length + 1, original_length + 1);

    for (size_t i = 0; i < diff_matrix->rows; i++)
    {
        unicorn_diff_matrix_set(diff_matrix, i, 0, i);
    }

    for (size_t j = 0; j < diff_matrix->columns; j++)
    {
        unicorn_diff_matrix_set(diff_matrix, 0, j, j);
    }

    return diff_matrix;
}

static void fill_different(UnicornDiffMatrix *diff_matrix, size_t i, size_t j)
{
    unicorn_diff_matrix_set(
        diff_matrix,
        i,
        j,
        unicorn_min_int(
            unicorn_diff_matrix_get(diff_matrix, i - 1, j - 1),
            unicorn_min_int(
                unicorn_diff_matrix_get(diff_matrix, i - 1, j),
                unicorn_diff_matrix_get(diff_matrix, i, j - 1)
            )
        ) + 1
    );
}

static void fill_equal(UnicornDiffMatrix *diff_matrix, size_t i, size_t j)
{
    unicorn_diff_matrix_set(
        diff_matrix,
        i,
        j,
        unicorn_diff_matrix_get(diff_matrix, i - 1, j - 1)
    );
}

void unicorn_diff_matrix_fill_from_strings(UnicornDiffMatrix *diff_matrix, char *original, char *modified)
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

void unicorn_diff_matrix_fill_from_lines(UnicornDiffMatrix *diff_matrix, char *original, char *modified)
{
    char *modified_pos;
    char *modified_line = modified;

    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        modified_pos = unicorn_next_line(modified_line);
        size_t modified_line_length = modified_pos - modified_line;

        char *original_pos;
        char *original_line = original;

        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            original_pos = unicorn_next_line(original_line);
            size_t original_line_length = original_pos - original_line;

            if (original_line_length == modified_line_length && strncmp(original_line, modified_line, original_line_length) == 0)
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

UnicornDiff unicorn_diff_matrix_get_diff(UnicornDiffMatrix *diff_matrix)
{
    if (diff_matrix->rows == 1 && diff_matrix->columns == 1)
    {
        UnicornDiff diff = { .size = 0, .chunks = NULL };
        return diff;
    }

    size_t capacity = 32;
    size_t size = 0;
    UnicornDiffChunk *backtrack = malloc(capacity * sizeof (UnicornDiffChunk));

    size_t i = diff_matrix->rows - 1;
    size_t j = diff_matrix->columns - 1;

    while (i > 0 || j > 0)
    {
        if (size == capacity)
        {
            capacity *= 2;
            backtrack = realloc(backtrack, capacity * sizeof (UnicornDiffChunk));
        }

        UnicornDiffChunk *current_chunk = &backtrack[size];
        size++;

        int current = unicorn_diff_matrix_get(diff_matrix, i, j);

        if (i > 0 && j > 0 && current == unicorn_diff_matrix_get(diff_matrix, i - 1, j - 1) + 1)
        {
            current_chunk->type = UNICORN_DIFF_CHUNK_TYPE_REPLACED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
        else if (j > 0 && current == unicorn_diff_matrix_get(diff_matrix, i, j - 1) + 1)
        {
            current_chunk->type = UNICORN_DIFF_CHUNK_TYPE_DELETED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i;
            current_chunk->modified_end = i;
            j--;
        }
        else if (i > 0 && current == unicorn_diff_matrix_get(diff_matrix, i - 1, j) + 1)
        {
            current_chunk->type = UNICORN_DIFF_CHUNK_TYPE_ADDED;
            current_chunk->original_start = j;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
        }
        else if (i > 0 && j > 0 && current == unicorn_diff_matrix_get(diff_matrix, i - 1, j - 1))
        {
            current_chunk->type = UNICORN_DIFF_CHUNK_TYPE_MATCHED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
    }

    UnicornDiff diff = { size, malloc(size * sizeof (UnicornDiffChunk)) };

    ssize_t backtrack_index = size - 1;
    size_t chunk_index = 0;

    diff.chunks[chunk_index] = backtrack[backtrack_index];

    for (backtrack_index--; backtrack_index >= 0; backtrack_index--)
    {
        UnicornDiffChunk *chunk = &backtrack[backtrack_index];
        UnicornDiffChunk *previous_chunk = &diff.chunks[chunk_index];

        if (chunk->type == previous_chunk->type)
        {
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else if ((chunk->type == UNICORN_DIFF_CHUNK_TYPE_REPLACED && previous_chunk->type != UNICORN_DIFF_CHUNK_TYPE_MATCHED) || (chunk->type != UNICORN_DIFF_CHUNK_TYPE_MATCHED && previous_chunk->type == UNICORN_DIFF_CHUNK_TYPE_REPLACED))
        {
            previous_chunk->type = UNICORN_DIFF_CHUNK_TYPE_REPLACED;
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
    diff.chunks = realloc(diff.chunks, diff.size * sizeof (UnicornDiffChunk));

    return diff;
}

size_t unicorn_diff_matrix_index(UnicornDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return row * diff_matrix->columns + column;
}

int unicorn_diff_matrix_get(UnicornDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return diff_matrix->content[unicorn_diff_matrix_index(diff_matrix, row, column)];
}

void unicorn_diff_matrix_set(UnicornDiffMatrix *diff_matrix, size_t row, size_t column, int value)
{
    diff_matrix->content[unicorn_diff_matrix_index(diff_matrix, row, column)] = value;
}


/*
 * Higher-level wrappers
 */

UnicornDiff unicorn_diff_strings_lengths(char *original, size_t original_length, char *modified, size_t modified_length)
{
    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix_from_lengths(original_length, modified_length);

    unicorn_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    UnicornDiff diff = unicorn_diff_matrix_get_diff(diff_matrix);

    unicorn_free_diff_matrix(diff_matrix);

    return diff;
}

UnicornDiff unicorn_diff_strings(char *original, char *modified)
{
    return unicorn_diff_strings_lengths(original, strlen(original), modified, strlen(modified));
}

UnicornDiff unicorn_diff_lines(char *original, char *modified)
{
    size_t original_length = unicorn_count_chars(original, '\n') + 1;
    size_t modified_length = unicorn_count_chars(modified, '\n') + 1;

    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix_from_lengths(original_length, modified_length);

    unicorn_diff_matrix_fill_from_lines(diff_matrix, original, modified);

    UnicornDiff diff = unicorn_diff_matrix_get_diff(diff_matrix);

    unicorn_free_diff_matrix(diff_matrix);

    return diff;
}


/*
 * Cleanup
 */

void unicorn_free_diff_matrix(UnicornDiffMatrix *diff_matrix)
{
    free(diff_matrix->content);
    free(diff_matrix);
}
#include <stdlib.h>

// #include "unicorn/unicorn.h"
#ifndef UNICORN_H
#define UNICORN_H


// #include "unicorn/types.h"


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

// #include "unicorn/diff/diff.h"

// #include "unicorn/fixture/fixture.h"

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

// #include "unicorn/result/result.h"

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

// #include "unicorn/test_utils/test_utils.h"



int unicorn_run_root_group(UnicornGroupItemRegistration *root_items, size_t item_count);


#define RUN_TESTS(...) \
    ({ \
        UnicornGroupItemRegistration _unicorn_root_items[] = { __VA_ARGS__ }; \
        unicorn_run_root_group(_unicorn_root_items, sizeof (_unicorn_root_items) / sizeof (*_unicorn_root_items)); \
    })


#endif



/*
 * Main run function
 */

int unicorn_run_root_group(UnicornGroupItemRegistration *root_items, size_t item_count)
{
    UnicornTestGroup *root_group = unicorn_new_test_group("root", root_items, item_count);
    UnicornTestSession *test_session = unicorn_new_test_session();

    unicorn_test_session_start(test_session);
    unicorn_test_session_run_test_group(test_session, root_group);
    unicorn_test_session_end(test_session);

    int status = test_session->failures->count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

    unicorn_free_test_group(root_group);
    unicorn_free_test_session(test_session);

    return status;
}
#include <stdlib.h>
#include <string.h>

// #include "unicorn/collection/collection.h"

// #include "unicorn/param/param.h"



/*
 * Currently accessible params
 */

UnicornCollection *_unicorn_current_params = NULL;


/*
 * Test param initialization
 */

static void initialize_test_param(UnicornTestParam *test_param, char *name, void *values, size_t count, UnicornTest *test)
{
    test_param->name = name;
    test_param->index = 0;
    test_param->count = count;
    test_param->values = values;
    test_param->test = test;
}

UnicornTestParam *unicorn_new_test_param(char *name, void *values, size_t count, UnicornTest *test)
{
    UnicornTestParam *test_param = malloc(sizeof (UnicornTestParam));
    initialize_test_param(test_param, name, values, count, test);

    return test_param;
}


/*
 * Get param from param collection
 */

UnicornTestParam *unicorn_get_test_param(UnicornCollection *params, char *param_name)
{
    UnicornTestParam *test_param;
    UNICORN_EACH(test_param, params)
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

void unicorn_free_test_param(UnicornTestParam *test_param)
{
    free(test_param);
}
#include <stdlib.h>

// #include "unicorn/collection/collection.h"

// #include "unicorn/group/group.h"

// #include "unicorn/test/test.h"



/*
 * Group creation
 */

static void initialize_test_group(UnicornTestGroup *test_group, char *name, UnicornGroupItemRegistration *group_items, size_t item_count)
{
    test_group->name = name;
    test_group->group = NULL;
    test_group->subgroups = unicorn_empty_collection();
    test_group->tests = unicorn_empty_collection();

    for (size_t i = 0; i < item_count; i++)
    {
        UnicornGroupItemRegistration registration = group_items[i];
        registration(test_group);
    }
}

UnicornTestGroup *unicorn_new_test_group(char *name, UnicornGroupItemRegistration *group_items, size_t item_count)
{
    UnicornTestGroup *test_group = malloc(sizeof (UnicornTestGroup));
    initialize_test_group(test_group, name, group_items, item_count);

    return test_group;
}


/*
 * Group items registration
 */

void unicorn_register_subgroup(UnicornTestGroup *test_group, char *name, UnicornGroupItemRegistration *group_items, size_t item_count)
{
    UnicornTestGroup *subgroup = unicorn_new_test_group(name, group_items, item_count);
    subgroup->group = test_group;

    unicorn_collection_append(test_group->subgroups, subgroup);
}

void unicorn_register_test(UnicornTestGroup *test_group, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    UnicornTest *test = unicorn_new_test(name, filename, line_number, function, test_modifiers, modifier_count);
    test->group = test_group;

    unicorn_collection_append(test_group->tests, test);
}


/*
 * Cleanup
 */

void unicorn_free_test_group(UnicornTestGroup *test_group)
{
    while (test_group->subgroups->count > 0)
    {
        UnicornTestGroup *subgroup = unicorn_collection_pop(test_group->subgroups);
        unicorn_free_test_group(subgroup);
    }
    unicorn_free_collection(test_group->subgroups);

    while (test_group->tests->count > 0)
    {
        UnicornTest *test = unicorn_collection_pop(test_group->tests);
        unicorn_free_test(test);
    }
    unicorn_free_collection(test_group->tests);

    free(test_group);
}
#include <stdlib.h>
#include <string.h>

// #include "unicorn/collection/collection.h"

// #include "unicorn/fixture/fixture.h"



/*
 * Currently accessible fixtures
 */

UnicornCollection *_unicorn_current_fixtures = NULL;


/*
 * Test fixture initialization
 */

static void initialize_test_fixture(UnicornTestFixture *test_fixture, char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTest *test, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    test_fixture->name = name;
    test_fixture->size = fixture_size;
    test_fixture->value = NULL;
    test_fixture->setup = setup;
    test_fixture->cleanup = NULL;
    test_fixture->test = test;
    test_fixture->accessible_fixtures = unicorn_empty_collection();
    test_fixture->accessible_params = unicorn_empty_collection();

    for (size_t i = 0; i < modifier_count; i++)
    {
        UnicornTestModifierRegistration registration = test_modifiers[i];
        registration(test_fixture->test, test_fixture->accessible_params, test_fixture->accessible_fixtures);
    }
}

UnicornTestFixture *unicorn_new_test_fixture(char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTest *test, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    UnicornTestFixture *test_fixture = malloc(sizeof (UnicornTestFixture));
    initialize_test_fixture(test_fixture, name, fixture_size, setup, test, test_modifiers, modifier_count);

    return test_fixture;
}


/*
 * Get fixture from fixture collection
 */

UnicornTestFixture *unicorn_get_test_fixture(UnicornCollection *fixtures, char *fixture_name)
{
    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, fixtures)
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

void unicorn_free_test_fixture(UnicornTestFixture *test_fixture)
{
    while (test_fixture->accessible_fixtures->count > 0)
    {
        unicorn_collection_pop(test_fixture->accessible_fixtures);
    }
    unicorn_free_collection(test_fixture->accessible_fixtures);

    while (test_fixture->accessible_params->count > 0)
    {
        unicorn_collection_pop(test_fixture->accessible_params);
    }
    unicorn_free_collection(test_fixture->accessible_params);

    free(test_fixture);
}
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

// #include "unicorn/collection/collection.h"

// #include "unicorn/diff/diff.h"

// #include "unicorn/group/group.h"

// #include "unicorn/output/output.h"

// #include "unicorn/output/ansi.h"
#ifndef UNICORN_OUTPUT_ANSI_H
#define UNICORN_OUTPUT_ANSI_H


#define INDENT "    "

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"

#define ANSI_BOLD   "\x1b[1m"
#define ANSI_RESET  "\x1b[0m"

#define COLOR(color, ...) \
    ANSI_RESET ANSI_COLOR_ ## color __VA_ARGS__ ANSI_RESET

#define BOLD(...) \
    ANSI_RESET ANSI_BOLD __VA_ARGS__ ANSI_RESET

#define COLOR_BOLD(color, ...) \
    ANSI_RESET ANSI_COLOR_ ## color ANSI_BOLD __VA_ARGS__ ANSI_RESET


#endif

// #include "unicorn/param/param.h"

// #include "unicorn/result/result.h"

// #include "unicorn/session/session.h"

// #include "unicorn/test/test.h"

// #include "unicorn/utils.h"



/*
 * Formatting utilities
 */

static void full_test_name(UnicornTest *test, char *full_name, size_t buffer_size)
{
    strncpy(full_name, test->name, buffer_size - 1);
    UnicornTestGroup *parent_group = test->group;

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

static void format_param_snapshot(UnicornTestParamSnapshot *param_snapshot, char *output_buffer, size_t buffer_size)
{
    snprintf(output_buffer, buffer_size, COLOR(MAGENTA, "%s") "[" COLOR(BLUE, "%zu") "]", param_snapshot->param->name, param_snapshot->index);
}

static void get_param_snapshots(UnicornCollectionItem *snapshot_item, char *output_buffer, size_t buffer_size)
{
    UnicornTestParamSnapshot *param_snapshot = snapshot_item->value;
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

static void display_test_result(UnicornTestResult *test_result)
{
    printf(INDENT);

    char full_name[256];
    full_test_name(test_result->test, full_name, sizeof (full_name));

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
        get_param_snapshots(test_result->param_snapshots->first, snapshot_string, sizeof (snapshot_string));
        printf(" " COLOR_BOLD(BLUE, "with"));
        printf(" %s", snapshot_string);
    }

    printf(" (" COLOR_BOLD(YELLOW, "%.2fms") ")", elapsed_milliseconds(test_result->start_time, test_result->end_time));

    printf("\n");
}

static void display_results(UnicornTestSession *test_session)
{
    printf("\nTest results:\n\n");

    UnicornTestResult *test_result;
    UNICORN_EACH(test_result, test_session->results)
    {
        display_test_result(test_result);
    }
}


/*
 * Display failures
 */

static void display_assertion(char *filename, size_t assertion_line)
{
    FILE *file = fopen(filename, "r");
    char line[256];
    size_t line_number = 0;

    size_t padding = 2;
    size_t before_assertion = assertion_line - padding;
    size_t after_assertion = assertion_line + padding;

    bool not_last_line = false;

    while(line_number < after_assertion && (not_last_line = fgets(line, sizeof (line), file) != NULL))
    {
        line_number++;

        if (line_number < before_assertion || after_assertion < line_number)
        {
            continue;
        }

        if (line_number == assertion_line)
        {
            char line_prefix[64];
            snprintf(line_prefix, sizeof (line_prefix), "> " COLOR_BOLD(MAGENTA, "%ld"), line_number);
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

static char *display_inline_diff(UnicornDiff *inline_diff, size_t lines, char *string, size_t *line_number, bool use_original)
{
    UnicornDiffChunk *inline_chunk = &inline_diff->chunks[0];

    size_t line_index = 0;
    size_t index = 0;

    for (size_t i = 0; i < lines; i++)
    {
        char *next = unicorn_next_line(string);
        size_t line_length = next - string;

        char line_prefix[64];

        if (use_original)
        {
            snprintf(line_prefix, sizeof (line_prefix), COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"), *line_number);
            printf("   %37s" COLOR(RED, " |  "), line_prefix);
        }
        else
        {
            snprintf(line_prefix, sizeof (line_prefix), COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"), *line_number);
            printf("   %37s" COLOR(GREEN, " |  "), line_prefix);
        }

        while (index - line_index < line_length)
        {
            size_t chunk_end = use_original ? inline_chunk->original_end : inline_chunk->modified_end;

            size_t start = index - line_index;
            size_t end = unicorn_min_size_t(chunk_end - line_index, line_length);

            size_t characters = end - start;

            if (inline_chunk->type == UNICORN_DIFF_CHUNK_TYPE_MATCHED)
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
                    printf(COLOR_BOLD(GREEN, "%.*s"), (int)characters, string + index - line_index);
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

    UnicornDiff diff = unicorn_diff_lines(original, modified);

    size_t line_number = 1;

    for (size_t chunk_index = 0; chunk_index < diff.size; chunk_index++)
    {
        UnicornDiffChunk *chunk = &diff.chunks[chunk_index];

        size_t original_lines = chunk->original_end - chunk->original_start;
        size_t modified_lines = chunk->modified_end - chunk->modified_start;

        if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_MATCHED)
        {
            for (size_t i = 0; i < original_lines; i++)
            {
                char *original_next = unicorn_next_line(original);
                char *modified_next = unicorn_next_line(modified);

                if (original_lines < 7 || (i < 2 && chunk_index > 0) || (original_lines - i < 3 && chunk_index < diff.size - 1))
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
        else if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_REPLACED)
        {
            char *original_end = unicorn_next_lines(original, original_lines);
            char *modified_end = unicorn_next_lines(modified, modified_lines);

            size_t original_length = original_end - original;
            size_t modified_length = modified_end - modified;

            UnicornDiff inline_diff = unicorn_diff_strings_lengths(original, original_length, modified, modified_length);

            original = display_inline_diff(&inline_diff, original_lines, original, &line_number, true);
            modified = display_inline_diff(&inline_diff, modified_lines, modified, &line_number, false);

            free(inline_diff.chunks);
        }
        else if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_DELETED)
        {
            for (size_t i = 0; i < original_lines; i++)
            {
                char *original_next = unicorn_next_line(original);

                char line_prefix[64];
                snprintf(line_prefix, sizeof (line_prefix), COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"), line_number);

                printf("   %37s", line_prefix);
                printf(COLOR(RED, " |  ") COLOR_BOLD(RED, "%.*s\n"), (int)(original_next - original), original);

                original = original_next + 1;
            }
        }
        else if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_ADDED)
        {
            for (size_t i = 0; i < modified_lines; i++)
            {
                char *modified_next = unicorn_next_line(modified);

                char line_prefix[64];
                snprintf(line_prefix, sizeof (line_prefix), COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"), line_number);

                printf("   %37s", line_prefix);
                printf(COLOR(GREEN, " |  ") COLOR_BOLD(GREEN, "%.*s\n"), (int)(modified_next - modified), modified);

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

    unicorn_output_string(stdout, output, 1, INDENT);
}

static void display_failure(UnicornTestResult *test_result)
{
    UnicornTest *test = test_result->test;

    char full_name[256];
    full_test_name(test, full_name, sizeof (full_name));

    printf("\n" INDENT BOLD("%s"), full_name);

    if (test_result->param_snapshots->count > 0)
    {
        char snapshot_string[256];
        get_param_snapshots(test_result->param_snapshots->first, snapshot_string, sizeof (snapshot_string));
        printf(" " COLOR_BOLD(BLUE, "with"));
        printf(" %s", snapshot_string);
    }

    printf(" failed:\n");

    printf("\n" INDENT INDENT "Location: " COLOR(GREEN, "%s:%zu") "\n", test_result->assertion_file, test_result->assertion_line);
    printf(INDENT INDENT "Time:     " COLOR_BOLD(YELLOW, "%.2fms") "\n", elapsed_milliseconds(test_result->start_time, test_result->end_time));
    printf(INDENT INDENT "Error:    ");

    if (test_result->failed_assertion != NULL)
    {
        printf(COLOR_BOLD(RED, "Assertion"));

        if (unicorn_is_short_string(test_result->failed_assertion))
        {
            printf(" " COLOR_BOLD(CYAN, "%s"), test_result->failed_assertion);
        }

        printf(COLOR_BOLD(RED, " failed."));
        printf("\n" INDENT INDENT INDENT INDENT "  ");
    }

    bool has_diff = unicorn_test_result_has_diff(test_result);

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

    if (test_result->assertion_line != test->line_number || strcmp(test_result->assertion_file, test->filename) != 0)
    {
        printf("\n");
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

static void display_failing_tests(UnicornTestSession *test_session)
{
    printf("\nFailing tests:\n");

    UnicornTestResult *test_result;
    UNICORN_EACH(test_result, test_session->failures)
    {
        display_failure(test_result);
    }
}


/*
 * Display session summary
 */

static void display_session_summary(UnicornTestSession *test_session)
{
    printf("\nTests: ");

    if (test_session->failures->count > 0)
    {
        printf(COLOR_BOLD(RED, "%zu failed") ", ", test_session->failures->count);
    }
    printf(COLOR_BOLD(GREEN, "%zu passed") ", ", test_session->results->count - test_session->failures->count);
    printf("%zu total\n", test_session->results->count);

    printf("Time:  " COLOR_BOLD(YELLOW, "%.2fms") "\n", elapsed_milliseconds(test_session->start_time, test_session->end_time));
}


/*
 * Progress utils
 */

static void display_dot_string(UnicornSessionOutputState *output_state)
{
    char string_label[64];

    snprintf(string_label, sizeof (string_label), COLOR(MAGENTA, "%d") " - " COLOR(MAGENTA, "%d"), output_state->index, output_state->index + output_state->length - 1);
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

void unicorn_output_string(FILE* stream, char *string, size_t line_number, char *indent)
{
    char *pos = strchr(string, '\n');
    char *line = string;

    while (pos != NULL)
    {
        fprintf(stream, "%s", indent);

        if (line_number > 0) {
            fprintf(stream, COLOR(MAGENTA, "%6zu"), line_number);
            fprintf(stream, " |  ");
        }

        fprintf(stream, "%.*s\n", (int)(pos - line), line);

        line = pos + 1;
        pos = strchr(line, '\n');

        if (line_number > 0) {
            line_number++;
        }
    }

    fprintf(stream, "%s", indent);

    if (line_number > 0) {
        fprintf(stream, COLOR(MAGENTA, "%6zu"), line_number);
        fprintf(stream, " |  ");
    }

    fprintf(stream, "%s\n", line);
}

void unicorn_output_session_init(UnicornTestSession *test_session)
{
    printf("\nRunning tests...\n");

    UnicornSessionOutputState *output_state = &test_session->output_state;

    output_state->length = sizeof (output_state->string);
    output_state->index = -output_state->length + 1;
}

void unicorn_output_session_progress(UnicornTestSession *test_session)
{
    UnicornSessionOutputState *output_state = &test_session->output_state;
    UnicornTestResult *last_result = test_session->results->last->value;

    if (output_state->length < (int)sizeof (output_state->string))
    {
        output_state->string[output_state->length] = last_result->success ? '.' : 'F';
        output_state->length++;

        printf("\r");
        display_dot_string(&test_session->output_state);
    }
    else
    {
        output_state->index += sizeof (output_state->string);
        output_state->length = 1;

        output_state->string[0] = last_result->success ? '.' : 'F';

        printf("\n");
        display_dot_string(&test_session->output_state);
    }
}

void unicorn_output_session_result(UnicornTestSession *test_session)
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
#include <stdlib.h>
#include <sys/time.h>

// #include "unicorn/collection/collection.h"

// #include "unicorn/group/group.h"

// #include "unicorn/output/output.h"

// #include "unicorn/param/param.h"

// #include "unicorn/result/result.h"

// #include "unicorn/session/session.h"

// #include "unicorn/test/test.h"



/*
 * Initialize test session
 */

static void initialize_test_session(UnicornTestSession *test_session)
{
    test_session->results = unicorn_empty_collection();
    test_session->failures = unicorn_empty_collection();
}

UnicornTestSession *unicorn_new_test_session()
{
    UnicornTestSession *test_session = malloc(sizeof (UnicornTestSession));
    initialize_test_session(test_session);

    return test_session;
}


/*
 * Test session hooks
 */

void unicorn_test_session_start(UnicornTestSession *test_session)
{
    gettimeofday(&test_session->start_time, NULL);

    unicorn_output_session_init(test_session);
}

void unicorn_test_session_end(UnicornTestSession *test_session)
{
    gettimeofday(&test_session->end_time, NULL);

    unicorn_output_session_result(test_session);
}


/*
 * Run test session
 */

static void register_result(UnicornTestSession *test_session, UnicornTestResult *test_result)
{
    unicorn_collection_append(test_session->results, test_result);

    if (!test_result->success)
    {
        unicorn_collection_append(test_session->failures, test_result);
    }
}

void unicorn_test_session_run_test(UnicornTestSession *test_session, UnicornTest *test)
{
    unicorn_run_test(test);
    register_result(test_session, test->result);

    unicorn_output_session_progress(test_session);
}

void unicorn_test_session_run_parameterized_test(UnicornTestSession *test_session, UnicornTest *test, UnicornCollectionItem *param_item)
{
    if (param_item == NULL)
    {
        unicorn_test_session_run_test(test_session, test);
        return;
    }

    UnicornTestParam *param = param_item->value;

    for (param->index = 0; param->index < param->count; param->index++)
    {
        unicorn_test_session_run_parameterized_test(test_session, test, param_item->next);
    }
}

void unicorn_test_session_run_test_group(UnicornTestSession *test_session, UnicornTestGroup *test_group)
{
    UnicornTestGroup *subgroup;
    UNICORN_EACH(subgroup, test_group->subgroups)
    {
        unicorn_test_session_run_test_group(test_session, subgroup);
    }

    UnicornTest *test;
    UNICORN_EACH(test, test_group->tests)
    {
        unicorn_test_session_run_parameterized_test(test_session, test, test->params->first);
    }
}


/*
 * Cleanup
 */

void unicorn_free_test_session(UnicornTestSession *test_session)
{
    while (test_session->results->count > 0)
    {
        UnicornTestResult *test_result = unicorn_collection_pop(test_session->results);
        unicorn_free_test_result(test_result);
    }
    unicorn_free_collection(test_session->results);

    while (test_session->failures->count > 0)
    {
        unicorn_collection_pop(test_session->failures);
    }
    unicorn_free_collection(test_session->failures);

    free(test_session);
}
#include <stdlib.h>

// #include "unicorn/collection/collection.h"



/*
 * Collection initialization
 */

static void initialize_collection(UnicornCollection *collection)
{
    collection->count = 0;
    collection->first = NULL;
    collection->last = NULL;
}

static void collection_item(UnicornCollectionItem *item, void *value)
{
    item->value = value;
    item->previous = NULL;
    item->next = NULL;
}

static UnicornCollectionItem *unicorn_new_collection_item(void *value)
{
    UnicornCollectionItem *item = malloc(sizeof (UnicornCollectionItem));
    collection_item(item, value);

    return item;
}

UnicornCollection *unicorn_empty_collection()
{
    UnicornCollection *collection = malloc(sizeof (UnicornCollection));
    initialize_collection(collection);

    return collection;
}


/*
 * Collection operations
 */

void unicorn_collection_append(UnicornCollection *collection, void *value)
{
    UnicornCollectionItem *new_item = unicorn_new_collection_item(value);
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

void *unicorn_collection_pop(UnicornCollection *collection)
{
    if (collection->count == 0)
    {
        return NULL;
    }

    UnicornCollectionItem *last_item = collection->last;
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

void unicorn_free_collection(UnicornCollection *collection)
{
    free(collection);
}
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

// #include "unicorn/test/test.h"

// #include "unicorn/test_utils/test_utils.h"

// #include "unicorn/utils.h"



UnicornOutputCapture _unicorn_default_output_capture =
{
    .initialization_phase = true,
    .stdout_backup = -1,
    .stderr_backup = -1,
    .pipe = { -1, -1 },
    .parent = NULL
};


/*
 * Initialize capture
 */

static void initialize_output_capture(UnicornOutputCapture *capture)
{
    if (pipe(capture->pipe) == -1)
    {
        fprintf(stderr, "Failed to create capture pipe.\n");
        exit(EXIT_FAILURE);
    }

    capture->parent = _unicorn_current_test->output_capture;
    _unicorn_current_test->output_capture = capture;

    capture->stdout_backup = dup(STDOUT_FILENO);
    capture->stderr_backup = dup(STDERR_FILENO);

    while (dup2(capture->pipe[1], STDOUT_FILENO) == -1 && errno == EINTR);
    while (dup2(capture->pipe[1], STDERR_FILENO) == -1 && errno == EINTR);
}


/*
 * Finalize capture
 */

static void finalize_output_capture(UnicornOutputCapture *capture, char **output_buffer)
{
    bool terminator_written = write(capture->pipe[1], "", 1) == 1;

    close(capture->pipe[1]);

    dup2(capture->stdout_backup, STDOUT_FILENO);
    dup2(capture->stderr_backup, STDERR_FILENO);

    if (terminator_written)
    {
        FILE *stream = fdopen(capture->pipe[0], "r");

        ssize_t output_length = unicorn_util_read_stream(stream, output_buffer) - 1;

        if (write(STDOUT_FILENO, *output_buffer, output_length) != output_length) {
            fprintf(stderr, "Failed to write captured output to stdout");
        }

        fclose(stream);
    }
    else
    {
        fprintf(stderr, "Failed to write to capture pipe.\n");
    }

    _unicorn_current_test->output_capture = capture->parent;

    close(capture->pipe[0]);
}


/*
 * Main capture function
 */

bool unicorn_capture_output(UnicornOutputCapture *capture, char **output_buffer)
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
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

// #include "unicorn/assertion/assertion.h"

// #include "unicorn/output/output.h"

// #include "unicorn/output/ansi.h"

// #include "unicorn/result/result.h"

// #include "unicorn/test/test.h"

// #include "unicorn/utils.h"



void unicorn_fail_test(UnicornTest *test, char *format, ...)
{
    size_t buffer_size;
    char *message;
    FILE *stream = open_memstream(&message, &buffer_size);

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fclose(stream);

    unicorn_pipe_error_message(test->result, message, buffer_size + 1);

    free(message);
}


bool unicorn_check_assertion(UnicornTest *test, bool assertion_success, char *assertion, char *assertion_file, size_t assertion_line)
{
    if (assertion_success)
    {
        return false;
    }

    unicorn_pipe_assertion_failure(test->result, assertion, assertion_file, assertion_line);
    return true;
}

bool unicorn_check_string_equal(char *actual, char *expected)
{
    if (strcmp(actual, expected) == 0)
    {
        return true;
    }

    UnicornTestResult *test_result = _unicorn_current_test->result;
    test_result->diff_original = expected;
    test_result->diff_original_size = strlen(expected) + 1;
    test_result->diff_modified = actual;
    test_result->diff_modified_size = strlen(actual) + 1;

    return false;
}


bool unicorn_check_substring(char *string, char *substring)
{
    return string != NULL && substring != NULL && strstr(string, substring) != NULL;
}


char *unicorn_assertion_process_string(char *string)
{
    if (unicorn_is_short_string(string))
    {
        return string;
    }

    size_t buffer_size;
    char *buffer;
    FILE *stream = open_memstream(&buffer, &buffer_size);

    fprintf(stream, "\n");
    unicorn_output_string(stream, string, 0, "  " INDENT INDENT INDENT INDENT INDENT ANSI_RESET);
    fprintf(stream, ANSI_COLOR_RED ANSI_BOLD "  " INDENT INDENT INDENT INDENT);

    fclose(stream);

    auto_free(buffer);

    return buffer;
}
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

// #include "unicorn/utils.h"



size_t unicorn_util_read_stream(FILE *stream, char **output_buffer)
{
    char buffer[256];
    size_t output_length = 0;

    ssize_t read_count = fread(buffer, 1, sizeof (buffer) - 1, stream);
    buffer[read_count] = '\0';

    if (read_count > 0)
    {
        *output_buffer = malloc(read_count + 1);
        memcpy(*output_buffer, buffer, read_count + 1);
    }

    output_length = read_count;

    while (read_count + 1 == sizeof (buffer))
    {
        read_count = fread(buffer, 1, sizeof (buffer) - 1, stream);
        buffer[read_count] = '\0';
        output_length += read_count;

        *output_buffer = realloc(*output_buffer, output_length + 1);
        memcpy(*output_buffer + output_length - read_count, buffer, read_count + 1);
    }

    return output_length;
}


bool unicorn_is_short_string(char *string)
{
    return strlen(string) < 64 && strchr(string, '\n') == NULL;
}

int unicorn_min_int(int a, int b)
{
    return a < b ? a : b;
}

size_t unicorn_min_size_t(size_t a, size_t b)
{
    return a < b ? a : b;
}

size_t unicorn_count_chars(char *string, char chr)
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

char *unicorn_next_line(char *string)
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

char *unicorn_next_lines(char *string, size_t lines)
{
    char *next_line = string;

    for (size_t i = 0; i < lines; i++)
    {
        next_line = unicorn_next_line(next_line) + 1;
    }

    return next_line;
}
