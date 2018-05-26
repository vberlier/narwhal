#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "unicorn/assertion/assertion.h"
#include "unicorn/result/result.h"
#include "unicorn/test/test.h"


void unicorn_fail_test(UnicornTest *test, char *format, ...)
{
    size_t buffer_size;
    char *message;
    FILE *stream = open_memstream(&message, &buffer_size);

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fclose(stream);

    unicorn_pipe_error_message(test->result, message, buffer_size + 1);

    free(message);
}


bool unicorn_check_assertion(UnicornTest *test, bool assertion_success, char *assertion, char *assertion_file, size_t assertion_line)
{
    if (assertion_success)
    {
        return false;
    }

    unicorn_pipe_assertion_failure(test->result, assertion, assertion_file, assertion_line);
    return true;
}


bool unicorn_check_substring(char *string, char *substring)
{
    return strstr(string, substring) != NULL;
}
