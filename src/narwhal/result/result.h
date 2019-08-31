#ifndef NARWHAL_RESULT_H
#define NARWHAL_RESULT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "narwhal/types.h"

struct NarwhalTestResult
{
    bool success;
    char *failed_assertion;
    char *error_message;
    char *assertion_file;
    size_t assertion_line;
    NarwhalTest *test;
    NarwhalCollection *param_snapshots;
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

NarwhalTestResult *narwhal_new_test_result();

bool narwhal_test_result_has_diff(const NarwhalTestResult *test_result);

void narwhal_pipe_test_info(NarwhalTestResult *test_result,
                            struct timeval start_time,
                            struct timeval end_time);
void narwhal_pipe_assertion_failure(NarwhalTestResult *test_result,
                                    const char *failed_assertion,
                                    const char *assertion_file,
                                    size_t assertion_line);
void narwhal_pipe_error_message(NarwhalTestResult *test_result,
                                const char *error_message,
                                size_t message_size);

void narwhal_set_assertion_failure(NarwhalTestResult *test_result,
                                   const char *failed_assertion,
                                   const char *assertion_file,
                                   size_t assertion_line);
void narwhal_set_error_message(NarwhalTestResult *test_result,
                               const char *error_message,
                               size_t message_size);
void narwhal_free_test_result(NarwhalTestResult *test_result);

struct NarwhalTestParamSnapshot
{
    const NarwhalTestParam *param;
    size_t index;
};

NarwhalTestParamSnapshot *narwhal_new_test_param_snapshot(const NarwhalTestParam *test_param);
void narwhal_free_test_param_snapshot(NarwhalTestParamSnapshot *param_snapshot);

#endif
