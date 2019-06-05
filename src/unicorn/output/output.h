#ifndef UNICORN_OUTPUT_H
#define UNICORN_OUTPUT_H


#include <stdio.h>

#include "unicorn/types.h"


void unicorn_output_string(FILE* stream, char *string, size_t line_number);

void unicorn_output_session_init(UnicornTestSession *test_session);
void unicorn_output_session_progress(UnicornTestSession *test_session);
void unicorn_output_session_result(UnicornTestSession *test_session);


#endif
