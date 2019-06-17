#ifndef UNICORN_UTILS_H
#define UNICORN_UTILS_H


#include <stdio.h>
#include <stdbool.h>


size_t unicorn_util_read_stream(FILE *stream, char **buffer);
bool unicorn_is_short_string(char *string);
int unicorn_min_int(int a, int b);
size_t unicorn_min_size_t(size_t a, size_t b);
size_t unicorn_count_chars(char *string, char chr);
char *unicorn_next_line(char *string);
char *unicorn_next_lines(char *string, size_t lines);


#endif
