#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>

#include "unicorn/utils.h"


size_t unicorn_util_read_stream(FILE *stream, char **output_buffer)
{
    char buffer[256];
    size_t output_length = 0;

    ssize_t read_count = fread(buffer, 1, sizeof (buffer) - 1, stream);
    buffer[read_count] = '\0';

    if (read_count > 0)
    {
        *output_buffer = malloc(read_count + 1);
        memcpy(*output_buffer, buffer, read_count + 1);
    }

    output_length = read_count;

    while (read_count + 1 == sizeof (buffer))
    {
        read_count = fread(buffer, 1, sizeof (buffer) - 1, stream);
        buffer[read_count] = '\0';
        output_length += read_count;

        *output_buffer = realloc(*output_buffer, output_length + 1);
        memcpy(*output_buffer + output_length - read_count, buffer, read_count + 1);
    }

    return output_length;
}


bool unicorn_is_short_string(char *string)
{
    return strlen(string) < 64 && strchr(string, '\n') == NULL;
}

int unicorn_min_int(int a, int b)
{
    return a < b ? a : b;
}

size_t unicorn_count_chars(char *string, char chr)
{
    size_t count = 0;

    for (size_t i = 0; string[i] != '\0'; i++)
    {
        if (string[i] == chr)
        {
            count++;
        }
    }

    return count;
}

char *unicorn_next_line(char *string)
{
    char *next_line = strchr(string, '\n');

    if (next_line != NULL)
    {
        return next_line;
    }
    else
    {
        return string + strlen(string);
    }
}
