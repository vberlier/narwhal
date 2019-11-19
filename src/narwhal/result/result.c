#include "narwhal/result/result.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#include "narwhal/collection/collection.h"
#include "narwhal/param/param.h"
#include "narwhal/test/test.h"
#include "narwhal/test_utils/test_utils.h"

/*
 * Test result initialization
 */

static void initialize_test_result(NarwhalTestResult *test_result)
{
    test_result->success = true;
    test_result->timed_out = false;
    test_result->failed_assertion = NULL;
    test_result->error_message = NULL;
    test_result->assertion_file = NULL;
    test_result->assertion_line = 0;
    test_result->test = NULL;
    test_result->param_snapshots = narwhal_empty_collection();
    test_result->output_buffer = NULL;
    test_result->output_length = 0;
    test_result->diff_original = NULL;
    test_result->diff_original_size = 0;
    test_result->diff_modified = NULL;
    test_result->diff_modified_size = 0;
}

NarwhalTestResult *narwhal_new_test_result(void)
{
    NarwhalTestResult *test_result = malloc(sizeof(NarwhalTestResult));
    initialize_test_result(test_result);

    return test_result;
}

/*
 * Util
 */

bool narwhal_test_result_has_diff(const NarwhalTestResult *test_result)
{
    return test_result->diff_original != NULL && test_result->diff_modified != NULL;
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
                                    const char *failed_assertion,
                                    const char *assertion_file,
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
                                const char *error_message,
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
                                   const char *failed_assertion,
                                   const char *assertion_file,
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
                               const char *error_message,
                               size_t message_size)
{
    test_result->success = false;
    test_result->error_message = malloc(message_size);
    strncpy(test_result->error_message, error_message, message_size);
}

/*
 * Create param snapshots
 */

static void initialize_test_param_snapshot(NarwhalTestParamSnapshot *param_snapshot,
                                           const NarwhalTestParam *test_param)
{
    param_snapshot->param = test_param;
    param_snapshot->index = test_param->index;
}

NarwhalTestParamSnapshot *narwhal_new_test_param_snapshot(const NarwhalTestParam *test_param)
{
    NarwhalTestParamSnapshot *param_snapshot = malloc(sizeof(NarwhalTestParamSnapshot));
    initialize_test_param_snapshot(param_snapshot, test_param);

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
