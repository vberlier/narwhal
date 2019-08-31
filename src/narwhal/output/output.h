#ifndef NARWHAL_OUTPUT_H
#define NARWHAL_OUTPUT_H

#include <stdio.h>

#include "narwhal/types.h"

void narwhal_output_string(FILE *stream,
                           const char *string,
                           size_t line_number,
                           const char *indent);

void narwhal_output_session_init(NarwhalTestSession *test_session);
void narwhal_output_session_progress(NarwhalTestSession *test_session);
void narwhal_output_session_result(const NarwhalTestSession *test_session);

#endif
