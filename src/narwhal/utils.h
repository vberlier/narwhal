#ifndef NARWHAL_UTILS_H
#define NARWHAL_UTILS_H


#include <stdio.h>
#include <stdbool.h>


size_t narwhal_util_read_stream(FILE *stream, char **buffer);
bool narwhal_is_short_string(char *string);
int narwhal_min_int(int a, int b);
size_t narwhal_min_size_t(size_t a, size_t b);
size_t narwhal_count_chars(char *string, char chr);
char *narwhal_next_line(char *string);
char *narwhal_next_lines(char *string, size_t lines);


#endif
