#include "narwhal/session/session.h"

#include <stdlib.h>
#include <sys/time.h>

#include "narwhal/collection/collection.h"
#include "narwhal/group/group.h"
#include "narwhal/output/output.h"
#include "narwhal/param/param.h"
#include "narwhal/result/result.h"
#include "narwhal/test/test.h"

/*
 * Initialize test session
 */

static void initialize_test_session(NarwhalTestSession *test_session)
{
    test_session->results = narwhal_empty_collection();
    test_session->failures = narwhal_empty_collection();
}

NarwhalTestSession *narwhal_new_test_session(void)
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
                                         NarwhalTestGroup *test_group,
                                         bool only)
{
    NarwhalTestGroup *subgroup;
    NARWHAL_EACH(subgroup, test_group->subgroups)
    {
        narwhal_test_session_run_test_group(test_session, subgroup, only);
    }

    NarwhalTest *test;
    NARWHAL_EACH(test, test_group->tests)
    {
        if (!only || test->only)
        {
            narwhal_test_session_run_parameterized_test(test_session, test, test->params->first);
        }
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
