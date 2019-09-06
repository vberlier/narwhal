#include "narwhal/assertion/assertion.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "narwhal/hexdump/hexdump.h"
#include "narwhal/output/ansi.h"
#include "narwhal/output/output.h"
#include "narwhal/result/result.h"
#include "narwhal/test/test.h"
#include "narwhal/utils.h"

void narwhal_fail_test(NarwhalTest *test, const char *format, ...)
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

bool narwhal_check_assertion(const NarwhalTest *test,
                             bool assertion_success,
                             const char *assertion,
                             const char *assertion_file,
                             size_t assertion_line)
{
    if (assertion_success)
    {
        return false;
    }

    narwhal_pipe_assertion_failure(test->result, assertion, assertion_file, assertion_line);
    return true;
}

bool narwhal_check_string_equal(const char *actual, const char *expected)
{
    if (strcmp(actual, expected) == 0)
    {
        return true;
    }

    NarwhalTestResult *test_result = _narwhal_current_test->result;
    test_result->diff_original = (char *)expected;           // Can't be const because the parent
    test_result->diff_original_size = strlen(expected) + 1;  // process needs to free the copies
    test_result->diff_modified = (char *)actual;             // piped from the test process
    test_result->diff_modified_size = strlen(actual) + 1;

    return false;
}

bool narwhal_check_substring(const char *string, const char *substring)
{
    return string != NULL && substring != NULL && strstr(string, substring) != NULL;
}

bool narwhal_check_memory_equal(const void *actual,
                                const void *expected,
                                size_t size,
                                size_t element_size)
{
    if (memcmp(actual, expected, size) == 0)
    {
        return true;
    }

    size_t bytes_per_row = narwhal_optimal_bytes_per_row(element_size, 16, 8);

    NarwhalTestResult *test_result = _narwhal_current_test->result;
    test_result->diff_original = narwhal_hexdump(expected, size, bytes_per_row);
    test_result->diff_original_size = strlen(test_result->diff_original) + 1;
    test_result->diff_modified = narwhal_hexdump(actual, size, bytes_per_row);
    test_result->diff_modified_size = strlen(test_result->diff_modified) + 1;

    return false;
}

const char *narwhal_assertion_process_string(const char *string)
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
