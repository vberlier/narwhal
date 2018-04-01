#ifndef UNICORN_SESSION_H
#define UNICORN_SESSION_H


#include <time.h>

#include "unicorn/types.h"


struct UnicornTestSession
{
    UnicornCollection *results;
    UnicornCollection *failures;
    clock_t start_time;
    clock_t end_time;
};

UnicornTestSession *unicorn_new_test_session();

void unicorn_test_session_start(UnicornTestSession *test_session);
void unicorn_test_session_end(UnicornTestSession *test_session);

void unicorn_test_session_run_test(UnicornTestSession *test_session, UnicornTest *test);
void unicorn_test_session_run_parameterized_test(UnicornTestSession *test_session, UnicornTest *test, UnicornCollectionItem *param_item);
void unicorn_test_session_run_test_group(UnicornTestSession *test_session, UnicornTestGroup *test_group);

void unicorn_free_test_session(UnicornTestSession *test_session);


#endif
