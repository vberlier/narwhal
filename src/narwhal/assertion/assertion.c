#include "narwhal/assertion/assertion.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "narwhal/output/ansi.h"
#include "narwhal/output/output.h"
#include "narwhal/result/result.h"
#include "narwhal/test/test.h"
#include "narwhal/utils.h"

void narwhal_fail_test(NarwhalTest *test, char *format, ...)
{
    size_t buffer_size;
    char *message;
    FILE *stream = open_memstream(&message, &buffer_size);

    va_list args;
    va_start(args, format);
    vfprintf(stream, format, args);
    va_end(args);

    fclose(stream);

    narwhal_pipe_error_message(test->result, message, buffer_size + 1);

    free(message);
}

bool narwhal_check_assertion(NarwhalTest *test,
                             bool assertion_success,
                             char *assertion,
                             char *assertion_file,
                             size_t assertion_line)
{
    if (assertion_success)
    {
        return false;
    }

    narwhal_pipe_assertion_failure(test->result, assertion, assertion_file, assertion_line);
    return true;
}

bool narwhal_check_string_equal(char *actual, char *expected)
{
    if (strcmp(actual, expected) == 0)
    {
        return true;
    }

    NarwhalTestResult *test_result = _narwhal_current_test->result;
    test_result->diff_original = expected;
    test_result->diff_original_size = strlen(expected) + 1;
    test_result->diff_modified = actual;
    test_result->diff_modified_size = strlen(actual) + 1;

    return false;
}

bool narwhal_check_substring(char *string, char *substring)
{
    return string != NULL && substring != NULL && strstr(string, substring) != NULL;
}

char *narwhal_assertion_process_string(char *string)
{
    if (narwhal_is_short_string(string))
    {
        return string;
    }

    size_t buffer_size;
    char *buffer;
    FILE *stream = open_memstream(&buffer, &buffer_size);

    fprintf(stream, "\n");
    narwhal_output_string(stream, string, 0, "  " INDENT INDENT INDENT INDENT INDENT ANSI_RESET);
    fprintf(stream, ANSI_COLOR_RED ANSI_BOLD "  " INDENT INDENT INDENT INDENT);

    fclose(stream);

    auto_free(buffer);

    return buffer;
}
