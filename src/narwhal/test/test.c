#include "narwhal/test/test.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "narwhal/collection/collection.h"
#include "narwhal/fixture/fixture.h"
#include "narwhal/param/param.h"
#include "narwhal/result/result.h"
#include "narwhal/unused_attribute.h"
#include "narwhal/utils.h"

/*
 * Current test
 */

NarwhalTest *_narwhal_current_test = NULL;

/*
 * Test creation
 */

static void initialize_test(NarwhalTest *test,
                            const char *name,
                            const char *filename,
                            size_t line_number,
                            NarwhalTestFunction function,
                            NarwhalTestModifierRegistration *test_modifiers,
                            size_t modifier_count,
                            NarwhalResetAllMocksFunction reset_all_mocks)
{
    test->name = name;
    test->filename = filename;
    test->line_number = line_number;
    test->only = false;
    test->skip = false;
    test->group = NULL;
    test->function = function;
    test->resources = narwhal_empty_collection();
    test->fixtures = narwhal_empty_collection();
    test->params = narwhal_empty_collection();
    test->accessible_fixtures = narwhal_empty_collection();
    test->accessible_params = narwhal_empty_collection();
    test->result = NULL;
    test->output_capture = NULL;
    test->reset_all_mocks = reset_all_mocks;

    for (size_t i = 0; i < modifier_count; i++)
    {
        NarwhalTestModifierRegistration registration = test_modifiers[i];
        registration(test, test->accessible_params, test->accessible_fixtures);
    }
}

NarwhalTest *narwhal_new_test(const char *name,
                              const char *filename,
                              size_t line_number,
                              NarwhalTestFunction function,
                              NarwhalTestModifierRegistration *test_modifiers,
                              size_t modifier_count,
                              NarwhalResetAllMocksFunction reset_all_mocks)
{
    NarwhalTest *test = malloc(sizeof(NarwhalTest));
    initialize_test(test,
                    name,
                    filename,
                    line_number,
                    function,
                    test_modifiers,
                    modifier_count,
                    reset_all_mocks);

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

static void test_error(NarwhalTestResult *test_result, const char *message, size_t message_size)
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

static void reset_all_mocks(NarwhalTest *test)
{
    if (test->reset_all_mocks != NULL)
    {
        test->reset_all_mocks();
    }
}

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

        reset_all_mocks(test);
        test_fixture->setup(test_fixture->value, test_fixture);
        reset_all_mocks(test);

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

            reset_all_mocks(test);
            test_fixture->cleanup(test_fixture->value, test_fixture);
            reset_all_mocks(test);

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

    reset_all_mocks(test);
    test->function();
    reset_all_mocks(test);

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
                                   const char *name,
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
                                 const char *name,
                                 const void *values,
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

void narwhal_test_set_only(NarwhalTest *test,
                           _NARWHAL_UNUSED NarwhalCollection *params,
                           _NARWHAL_UNUSED NarwhalCollection *fixtures)
{
    test->only = true;
}

void narwhal_test_set_skip(NarwhalTest *test,
                           _NARWHAL_UNUSED NarwhalCollection *params,
                           _NARWHAL_UNUSED NarwhalCollection *fixtures)
{
    test->skip = true;
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
