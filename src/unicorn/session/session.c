#include <stdlib.h>
#include <time.h>

#include "unicorn/collection/collection.h"
#include "unicorn/group/group.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"
#include "unicorn/session/session.h"
#include "unicorn/test/test.h"


/*
 * Initialize test session
 */

static void initialize_test_session(UnicornTestSession *test_session)
{
    test_session->results = unicorn_empty_collection();
    test_session->failures = unicorn_empty_collection();
    test_session->start_time = 0;
    test_session->end_time = 0;
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
    test_session->start_time = clock();
}

void unicorn_test_session_end(UnicornTestSession *test_session)
{
    test_session->end_time = clock();
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
