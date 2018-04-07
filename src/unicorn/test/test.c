#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "unicorn/collection/collection.h"
#include "unicorn/fixture/fixture.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"
#include "unicorn/test/test.h"


/*
 * Test creation
 */

static void initialize_test(UnicornTest *test, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestItemRegistration *test_items, size_t item_count)
{
    test->name = name;
    test->filename = filename;
    test->line_number = line_number;
    test->group = NULL;
    test->function = function;
    test->resources = unicorn_empty_collection();
    test->fixtures = unicorn_empty_collection();
    test->params = unicorn_empty_collection();
    test->result = NULL;

    for (size_t i = 0; i < item_count; i++)
    {
        UnicornTestItemRegistration registration = test_items[i];
        registration(test);
    }
}

UnicornTest *unicorn_new_test(char *name, char *filename, size_t line_number, UnicornTestFunction function, UnicornTestItemRegistration *test_items, size_t item_count)
{
    UnicornTest *test = malloc(sizeof (UnicornTest));
    initialize_test(test, name, filename, line_number, function, test_items, item_count);

    return test;
}


/*
 * Test resources
 */

void unicorn_free_after_test(UnicornTest *test, void *resource)
{
    unicorn_collection_append(test->resources, resource);
}

void *unicorn_test_resource(UnicornTest *test, size_t size)
{
    void *resource = malloc(size);
    unicorn_free_after_test(test, resource);
    return resource;
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
 * Run test
 */

static void setup_test_result(UnicornTest *test)
{
    test->result = unicorn_new_test_result();
    test->result->test = test;

    pipe(test->result->pipe);

    UnicornTestParam *test_param;
    UNICORN_EACH(test_param, test->params)
    {
        UnicornTestParamSnapshot *param_snapshot = unicorn_new_test_param_snapshot(test_param);
        unicorn_collection_append(test->result->param_snapshots, param_snapshot);
    }
}

static void test_start(UnicornTest *test)
{
    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        test_fixture->cleanup = NULL;
        test_fixture->value = unicorn_test_resource(test, test_fixture->size);
        test_fixture->setup(test_fixture->value, test_fixture);
    }
}

static void test_end(UnicornTest *test)
{
    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        if (test_fixture->cleanup != NULL)
        {
            test_fixture->cleanup(test_fixture->value, test_fixture);
        }
    }

    unicorn_free_test_resources(test);
}

static int execute_test_function(UnicornTest *test)
{
    test_start(test);
    struct timeval start_time;
    gettimeofday(&start_time, NULL);

    test->function(test, test);

    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    test_end(test);

    unicorn_pipe_duration(test->result, start_time, end_time);

    return test->result->success ? EXIT_SUCCESS : EXIT_FAILURE;
}

static void report_duration(UnicornTestResult *test_result)
{
    read(test_result->pipe[0], &test_result->start_time, sizeof (struct timeval));
    read(test_result->pipe[0], &test_result->end_time, sizeof (struct timeval));
}

static void report_failure(UnicornTestResult *test_result)
{
    size_t assertion_size;
    ssize_t bytes_read = read(test_result->pipe[0], &assertion_size, sizeof (size_t));

    if (bytes_read != sizeof (size_t))
    {
        char message[] = "Test process exited unexpectedly.";
        unicorn_set_assertion_failure(test_result, NULL, test_result->test->line_number);
        unicorn_set_error_message(test_result, message, sizeof (message));

        gettimeofday(&test_result->end_time, NULL);
        return;
    }

    if (assertion_size > 0)
    {
        test_result->failed_assertion = malloc(assertion_size);
        read(test_result->pipe[0], test_result->failed_assertion, assertion_size);
    }
    else
    {
        test_result->failed_assertion = NULL;
    }

    read(test_result->pipe[0], &test_result->assertion_line, sizeof (size_t));

    size_t message_size;
    read(test_result->pipe[0], &message_size, sizeof (size_t));

    test_result->error_message = malloc(message_size);
    read(test_result->pipe[0], test_result->error_message, message_size);

    report_duration(test_result);
}

void unicorn_run_test(UnicornTest *test)
{
    setup_test_result(test);

    pid_t test_pid = fork();

    if (test_pid == -1)
    {
        char message[] = "Couldn't create the test child process.";
        unicorn_set_assertion_failure(test->result, NULL, test->line_number);
        unicorn_set_error_message(test->result, message, sizeof (message));
    }
    else if (test_pid == 0)
    {
        close(test->result->pipe[0]);

        int test_status = execute_test_function(test);

        close(test->result->pipe[1]);
        exit(test_status);
    }
    else
    {
        gettimeofday(&test->result->start_time, NULL);

        close(test->result->pipe[1]);

        int test_status;
        waitpid(test_pid, &test_status, 0);

        test->result->success = test_status == EXIT_SUCCESS;

        if (test->result->success)
        {
            report_duration(test->result);
        }
        else
        {
            report_failure(test->result);
        }

        close(test->result->pipe[0]);
    }
}


/*
 * Test fixtures
 */

void unicorn_register_test_fixture(UnicornTest *test, char *name, size_t fixture_size, UnicornTestFixtureSetup setup)
{
    UnicornTestFixture *test_fixture = unicorn_new_test_fixture(name, fixture_size, setup);
    test_fixture->test = test;

    unicorn_collection_append(test->fixtures, test_fixture);
}

UnicornTestFixture *unicorn_get_test_fixture(UnicornTest *test, char *fixture_name)
{
    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        if (strcmp(test_fixture->name, fixture_name) == 0)
        {
            return test_fixture;
        }
    }
    return NULL;
}


/*
 * Test params
 */

void unicorn_register_test_param(UnicornTest *test, char *name, void *values, size_t count)
{
    UnicornTestParam *test_param = unicorn_new_test_param(name, values, count);
    test_param->test = test;

    unicorn_collection_append(test->params, test_param);
}

UnicornTestParam *unicorn_get_test_param(UnicornTest *test, char *param_name)
{
    UnicornTestParam *test_param;
    UNICORN_EACH(test_param, test->params)
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

void unicorn_free_test(UnicornTest *test)
{
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
