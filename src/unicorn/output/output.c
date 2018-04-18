#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

#include "unicorn/collection/collection.h"
#include "unicorn/group/group.h"
#include "unicorn/output/output.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"
#include "unicorn/session/session.h"
#include "unicorn/test/test.h"


#define INDENT "    "

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"

#define ANSI_BOLD   "\x1b[1m"
#define ANSI_RESET  "\x1b[0m"

#define COLOR(color, ...) \
    ANSI_COLOR_ ## color __VA_ARGS__ ANSI_RESET

#define BOLD(...) \
    ANSI_BOLD __VA_ARGS__ ANSI_RESET

#define COLOR_BOLD(color, ...) \
    ANSI_COLOR_ ## color ANSI_BOLD __VA_ARGS__ ANSI_RESET


/*
 * Formatting utilities
 */

static void full_test_name(UnicornTest *test, char *full_name, size_t buffer_size)
{
    strncpy(full_name, test->name, buffer_size);
    UnicornTestGroup *parent_group = test->group;

    if (parent_group != NULL)
    {
        while (parent_group->group != NULL)
        {
            char new_name[buffer_size];
            snprintf(new_name, buffer_size, "%s/%s", parent_group->name, full_name);
            strncpy(full_name, new_name, buffer_size);
            parent_group = parent_group->group;
        }
    }
}

static void format_param_snapshot(UnicornTestParamSnapshot *param_snapshot, char *output_buffer, size_t buffer_size)
{
    snprintf(output_buffer, buffer_size, COLOR(MAGENTA, "%s") "[" COLOR(BLUE, "%zu") "]", param_snapshot->param->name, param_snapshot->index);
}

static void get_param_snapshots(UnicornCollectionItem *snapshot_item, char *output_buffer, size_t buffer_size)
{
    UnicornTestParamSnapshot *param_snapshot = snapshot_item->value;
    format_param_snapshot(param_snapshot, output_buffer, buffer_size);

    snapshot_item = snapshot_item->next;

    if (snapshot_item == NULL)
    {
        return;
    }

    param_snapshot = snapshot_item->value;

    while (snapshot_item->next != NULL)
    {
        char snapshot_string[buffer_size];
        format_param_snapshot(param_snapshot, snapshot_string, buffer_size);

        char new_value[buffer_size];
        snprintf(new_value, buffer_size, "%s, %s", output_buffer, snapshot_string);
        strncpy(output_buffer, new_value, buffer_size);

        snapshot_item = snapshot_item->next;
        param_snapshot = snapshot_item->value;
    }

    char snapshot_string[buffer_size];
    format_param_snapshot(param_snapshot, snapshot_string, buffer_size);

    char new_value[buffer_size];
    snprintf(new_value, buffer_size, "%s and %s", output_buffer, snapshot_string);
    strncpy(output_buffer, new_value, buffer_size);
}

static double elapsed_milliseconds(struct timeval start_time, struct timeval end_time)
{
    double milliseconds = (end_time.tv_sec) * 1000.0 + (end_time.tv_usec) / 1000.0;
    milliseconds -= (start_time.tv_sec) * 1000.0 + (start_time.tv_usec) / 1000.0;
    return milliseconds;
}


/*
 * Display result list
 */

static void display_test_result(UnicornTestResult *test_result)
{
    printf(INDENT);

    char full_name[256];
    full_test_name(test_result->test, full_name, sizeof (full_name));

    if (test_result->success)
    {
        printf(COLOR_BOLD(GREEN, "PASS") " %s", full_name);
    }
    else
    {
        printf(COLOR_BOLD(RED, "FAIL") " " BOLD("%s"), full_name);
    }

    if (test_result->param_snapshots->count > 0)
    {
        char snapshot_string[256];
        get_param_snapshots(test_result->param_snapshots->first, snapshot_string, sizeof (snapshot_string));
        printf(" " COLOR_BOLD(BLUE, "with"));
        printf(" %s", snapshot_string);
    }

    printf(" (" COLOR_BOLD(YELLOW, "%.2fms") ")", elapsed_milliseconds(test_result->start_time, test_result->end_time));

    printf("\n");
}

static void display_results(UnicornTestSession *test_session)
{
    printf("\nTest results:\n\n");

    UnicornTestResult *test_result;
    UNICORN_EACH(test_result, test_session->results)
    {
        display_test_result(test_result);
    }
}


/*
 * Display failures
 */

static void display_assertion(char *filename, size_t assertion_line)
{
    FILE *file = fopen(filename, "r");
    char line[256];
    size_t line_number = 0;

    size_t padding = 2;
    size_t before_assertion = assertion_line - padding;
    size_t after_assertion = assertion_line + padding;

    bool not_last_line = false;

    while(line_number < after_assertion && (not_last_line = fgets(line, sizeof (line), file) != NULL))
    {
        line_number++;

        if (line_number < before_assertion || after_assertion < line_number)
        {
            continue;
        }

        if (line_number == assertion_line)
        {
            char line_prefix[32];
            snprintf(line_prefix, sizeof (line_prefix), "> " COLOR_BOLD(MAGENTA, "%ld"), line_number);
            printf("    %19s", line_prefix);
            printf(" |  " COLOR_BOLD(CYAN, "%s"), line);
        }
        else
        {
            printf("    " COLOR(MAGENTA, "%6zu"), line_number);
            printf(" |  %s", line);
        }
    }

    if (!not_last_line)
    {
        printf("\n");
    }

    fclose(file);
}

static void display_output(char *output)
{
    printf(INDENT INDENT "Output:\n\n");

    char *pos = strchr(output, '\n');
    char *line = output;

    size_t line_number = 0;

    while (pos != NULL)
    {
        line_number++;
        printf("    " COLOR(MAGENTA, "%6zu"), line_number);

        *pos = '\0';
        printf(" |  %s\n", line);
        *pos = '\n';

        line = pos + 1;
        pos = strchr(line, '\n');
    }

    printf("    " COLOR(MAGENTA, "%6zu"), line_number + 1);
    printf(" |  %s\n", line);
}

static void display_failure(UnicornTestResult *test_result)
{
    UnicornTest *test = test_result->test;

    char full_name[256];
    full_test_name(test, full_name, sizeof (full_name));

    printf("\n" INDENT BOLD("%s"), full_name);

    if (test_result->param_snapshots->count > 0)
    {
        char snapshot_string[256];
        get_param_snapshots(test_result->param_snapshots->first, snapshot_string, sizeof (snapshot_string));
        printf(" " COLOR_BOLD(BLUE, "with"));
        printf(" %s", snapshot_string);
    }

    printf(" failed:\n");

    printf("\n" INDENT INDENT "Location: " COLOR(GREEN, "%s:%zu") "\n", test_result->test->filename, test_result->assertion_line);
    printf(INDENT INDENT "Time:     " COLOR_BOLD(YELLOW, "%.2fms") "\n", elapsed_milliseconds(test_result->start_time, test_result->end_time));
    printf(INDENT INDENT "Error:    ");

    if (test_result->failed_assertion != NULL)
    {
        printf(COLOR_BOLD(RED, "Assertion ") COLOR_BOLD(CYAN, "%s") COLOR_BOLD(RED, " failed."), test_result->failed_assertion);
        printf("\n" INDENT INDENT INDENT INDENT "  ");
    }

    if (strlen(test_result->error_message) > 0)
    {
        printf(COLOR_BOLD(RED, "%s"), test_result->error_message);
    }
    else
    {
        printf(COLOR_BOLD(RED, "%s"), "No details available.");
    }

    printf("\n");

    if (test_result->assertion_line != test->line_number)
    {
        printf("\n");
        display_assertion(test->filename, test_result->assertion_line);
    }

    if (test->output_length > 0)
    {
        printf("\n");
        display_output(test->output_buffer);
    }
}

static void display_failing_tests(UnicornTestSession *test_session)
{
    printf("\nFailing tests:\n");

    UnicornTestResult *test_result;
    UNICORN_EACH(test_result, test_session->failures)
    {
        display_failure(test_result);
    }
}


/*
 * Display session summary
 */

static void display_session_summary(UnicornTestSession *test_session)
{
    printf("\nTests: ");

    if (test_session->failures->count > 0)
    {
        printf(COLOR_BOLD(RED, "%zu failed") ", ", test_session->failures->count);
    }
    printf(COLOR_BOLD(GREEN, "%zu passed") ", ", test_session->results->count - test_session->failures->count);
    printf("%zu total\n", test_session->results->count);

    printf("Time:  " COLOR_BOLD(YELLOW, "%.2fms") "\n", elapsed_milliseconds(test_session->start_time, test_session->end_time));
}


/*
 * Main output function
 */

void unicorn_output_session_result(UnicornTestSession *test_session)
{
    if (test_session->results->count > 0)
    {
        display_results(test_session);
    }

    if (test_session->failures->count > 0)
    {
        display_failing_tests(test_session);
    }

    display_session_summary(test_session);
}
