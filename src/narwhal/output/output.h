#ifndef NARWHAL_OUTPUT_H
#define NARWHAL_OUTPUT_H

#include <stdio.h>

#include "narwhal/types.h"

void narwhal_output_string(FILE *stream, char *string, size_t line_number, char *indent);

void narwhal_output_session_init(NarwhalTestSession *test_session);
void narwhal_output_session_progress(NarwhalTestSession *test_session);
void narwhal_output_session_result(NarwhalTestSession *test_session);

#endif
