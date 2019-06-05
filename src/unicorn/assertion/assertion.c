#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdarg.h>

#include "unicorn/assertion/assertion.h"
#include "unicorn/output/output.h"
#include "unicorn/output/ansi.h"
#include "unicorn/result/result.h"
#include "unicorn/test/test.h"
#include "unicorn/utils.h"


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
    return string != NULL && substring != NULL && strstr(string, substring) != NULL;
}


char *unicorn_assertion_process_string(char *string)
{
    if (unicorn_is_short_string(string))
    {
        return string;
    }

    size_t buffer_size;
    char *buffer;
    FILE *stream = open_memstream(&buffer, &buffer_size);

    fprintf(stream, "\n");
    unicorn_output_string(stream, string, 0, "  " INDENT INDENT INDENT INDENT INDENT ANSI_RESET);
    fprintf(stream, ANSI_COLOR_RED ANSI_BOLD "  " INDENT INDENT INDENT INDENT);

    fclose(stream);

    auto_free(buffer);

    return buffer;
}
