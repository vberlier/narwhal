#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

#include "unicorn/collection/collection.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"


/*
 * Test result initialization
 */

static void initialize_test_result(UnicornTestResult *test_result)
{
    test_result->success = true;
    test_result->failed_assertion = NULL;
    test_result->error_message = NULL;
    test_result->assertion_line = 0;
    test_result->test = NULL;
    test_result->param_snapshots = unicorn_empty_collection();
}

UnicornTestResult *unicorn_new_test_result()
{
    UnicornTestResult *test_result = malloc(sizeof (UnicornTestResult));
    initialize_test_result(test_result);

    return test_result;
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

void unicorn_pipe_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, size_t assertion_line)
{
    test_result->success = false;
    push_data(&test_result->success, sizeof (bool));

    size_t assertion_size = failed_assertion != NULL ? strlen(failed_assertion) + 1 : 0;
    push_data(&assertion_size, sizeof (assertion_size));
    push_data(failed_assertion, assertion_size);
    push_data(&assertion_line, sizeof (assertion_line));
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

void unicorn_set_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, size_t assertion_line)
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

    if (test_result->failed_assertion != NULL)
    {
        free(test_result->failed_assertion);
    }

    free(test_result->error_message);
    free(test_result);
}
