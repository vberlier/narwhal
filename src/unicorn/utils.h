#ifndef UNICORN_UTILS_H
#define UNICORN_UTILS_H


#include <stdio.h>
#include <stdbool.h>


size_t unicorn_util_read_stream(FILE *stream, char **buffer);
bool unicorn_is_short_string(char *string);


#endif
