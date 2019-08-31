#ifndef NARWHAL_UTILS_H
#define NARWHAL_UTILS_H

#include <stdbool.h>
#include <stdio.h>

size_t narwhal_util_read_stream(FILE *stream, char **buffer);
bool narwhal_is_short_string(const char *string);
int narwhal_min_int(int a, int b);
size_t narwhal_min_size_t(size_t a, size_t b);
size_t narwhal_count_chars(const char *string, char chr);
const char *narwhal_next_line(const char *string);
const char *narwhal_next_lines(const char *string, size_t lines);

#endif
