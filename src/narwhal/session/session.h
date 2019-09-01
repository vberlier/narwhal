#ifndef NARWHAL_SESSION_H
#define NARWHAL_SESSION_H

#include <sys/time.h>

#include "narwhal/types.h"

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

NarwhalTestSession *narwhal_new_test_session(void);

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
