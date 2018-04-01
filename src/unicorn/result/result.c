#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

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
    test_result->start_time = 0;
    test_result->end_time = 0;
}

UnicornTestResult *unicorn_new_test_result()
{
    UnicornTestResult *test_result = malloc(sizeof (UnicornTestResult));
    initialize_test_result(test_result);

    return test_result;
}


/*
 * Set failing test result
 */

void unicorn_set_assertion_failure(UnicornTestResult *test_result, char *failed_assertion, size_t assertion_line)
{
    test_result->failed_assertion = failed_assertion;
    test_result->assertion_line = assertion_line;
}

void unicorn_set_error_message(UnicornTestResult *test_result, char *error_message, size_t message_size)
{
    test_result->success = false;
    test_result->error_message = malloc(message_size);
    snprintf(test_result->error_message, message_size, error_message);
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
    free(test_result->error_message);
    free(test_result);
}
