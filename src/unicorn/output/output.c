#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

#include "unicorn/collection/collection.h"
#include "unicorn/diff/diff.h"
#include "unicorn/group/group.h"
#include "unicorn/output/output.h"
#include "unicorn/output/ansi.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"
#include "unicorn/session/session.h"
#include "unicorn/test/test.h"
#include "unicorn/utils.h"


/*
 * Formatting utilities
 */

static void full_test_name(UnicornTest *test, char *full_name, size_t buffer_size)
{
    strncpy(full_name, test->name, buffer_size - 1);
    UnicornTestGroup *parent_group = test->group;

    if (parent_group != NULL)
    {
        while (parent_group->group != NULL)
        {
            char current[buffer_size];
            memcpy(current, full_name, buffer_size);
            strncpy(full_name, parent_group->name, buffer_size - 1);
            strncat(full_name, "/", buffer_size - 1);
            strncat(full_name, current, buffer_size - 1);
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

    strncat(output_buffer, " and ", buffer_size);
    strncat(output_buffer, snapshot_string, buffer_size);
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

static char *display_inline_diff(UnicornDiff *inline_diff, size_t lines, char *string, size_t *line_number, bool use_original)
{
    UnicornDiffChunk *inline_chunk = &inline_diff->chunks[0];

    size_t line_index = 0;
    size_t index = 0;

    for (size_t i = 0; i < lines; i++)
    {
        char *next = unicorn_next_line(string);
        size_t line_length = next - string;

        char line_prefix[32];

        if (use_original)
        {
            snprintf(line_prefix, sizeof (line_prefix), COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"), *line_number);
            printf("   %29s" COLOR(RED, " |  "), line_prefix);
        }
        else
        {
            snprintf(line_prefix, sizeof (line_prefix), COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"), *line_number);
            printf("   %29s" COLOR(GREEN, " |  "), line_prefix);
        }

        while (index - line_index < line_length)
        {
            size_t start = index - line_index;
            size_t end = unicorn_min_size_t((use_original ? inline_chunk->original_end : inline_chunk->modified_end) - line_index, line_length);

            size_t characters = end - start;

            if (inline_chunk->type == UNICORN_DIFF_CHUNK_TYPE_MATCHED)
            {
                printf("%.*s", (int)(end - start), string + index - line_index);
            }
            else
            {
                if (use_original)
                {
                    printf(COLOR_BOLD(RED, "%.*s"), (int)(end - start), string + index - line_index);
                }
                else
                {
                    printf(COLOR_BOLD(GREEN, "%.*s"), (int)(end - start), string + index - line_index);
                }
            }

            index += characters;

            if (index >= (use_original ? inline_chunk->original_end : inline_chunk->modified_end))
            {
                inline_chunk++;
            }
        }

        printf("\n");

        if (!use_original)
        {
            (*line_number)++;
        }

        string = next + 1;
        line_index += line_length + 1;
        index = line_index;
    }

    return string;
}

static void display_diff(char *original, char *modified)
{
    printf(INDENT INDENT "Diff:\n\n");

    UnicornDiff diff = unicorn_diff_lines(original, modified);

    size_t line_number = 1;

    for (size_t chunk_index = 0; chunk_index < diff.size; chunk_index++)
    {
        UnicornDiffChunk *chunk = &diff.chunks[chunk_index];

        size_t original_lines = chunk->original_end - chunk->original_start;
        size_t modified_lines = chunk->modified_end - chunk->modified_start;

        if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_MATCHED)
        {
            for (size_t i = 0; i < original_lines; i++)
            {
                char *original_next = unicorn_next_line(original);
                char *modified_next = unicorn_next_line(modified);

                if (original_lines < 7 || (i < 2 && chunk_index > 0) || (original_lines - i < 3 && chunk_index < diff.size - 1))
                {
                    printf(INDENT COLOR(MAGENTA, "%6zu"), line_number);
                    printf(" |  %.*s\n", (int)(original_next - original), original);
                }
                else if (i == 2)
                {
                    printf(INDENT INDENT "   :\n");
                }

                line_number++;
                original = original_next + 1;
                modified = modified_next + 1;
            }
        }
        else if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_REPLACED)
        {
            char *original_end = unicorn_next_lines(original, original_lines);
            char *modified_end = unicorn_next_lines(modified, modified_lines);

            size_t original_length = original_end - original;
            size_t modified_length = modified_end - modified;

            UnicornDiff inline_diff = unicorn_diff_strings_lengths(original, original_length, modified, modified_length);

            original = display_inline_diff(&inline_diff, original_lines, original, &line_number, true);
            modified = display_inline_diff(&inline_diff, modified_lines, modified, &line_number, false);

            free(inline_diff.chunks);
        }
        else if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_DELETED)
        {
            for (size_t i = 0; i < original_lines; i++)
            {
                char *original_next = unicorn_next_line(original);

                char line_prefix[32];
                snprintf(line_prefix, sizeof (line_prefix), COLOR(RED, "- ") COLOR_BOLD(RED, "%ld"), line_number);

                printf("   %29s", line_prefix);
                printf(COLOR(RED, " |  ") COLOR_BOLD(RED, "%.*s\n"), (int)(original_next - original), original);

                original = original_next + 1;
            }
        }
        else if (chunk->type == UNICORN_DIFF_CHUNK_TYPE_ADDED)
        {
            for (size_t i = 0; i < modified_lines; i++)
            {
                char *modified_next = unicorn_next_line(modified);

                char line_prefix[32];
                snprintf(line_prefix, sizeof (line_prefix), COLOR(GREEN, "+ ") COLOR_BOLD(GREEN, "%ld"), line_number);

                printf("   %29s", line_prefix);
                printf(COLOR(GREEN, " |  ") COLOR_BOLD(GREEN, "%.*s\n"), (int)(modified_next - modified), modified);

                line_number++;
                modified = modified_next + 1;
            }
        }
    }

    free(diff.chunks);
}

static void display_output(char *output)
{
    printf(INDENT INDENT "Output:\n\n");

    unicorn_output_string(stdout, output, 1, INDENT);
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

    printf("\n" INDENT INDENT "Location: " COLOR(GREEN, "%s:%zu") "\n", test_result->assertion_file, test_result->assertion_line);
    printf(INDENT INDENT "Time:     " COLOR_BOLD(YELLOW, "%.2fms") "\n", elapsed_milliseconds(test_result->start_time, test_result->end_time));
    printf(INDENT INDENT "Error:    ");

    if (test_result->failed_assertion != NULL)
    {
        printf(COLOR_BOLD(RED, "Assertion"));

        if (unicorn_is_short_string(test_result->failed_assertion))
        {
            printf(" " COLOR_BOLD(CYAN, "%s"), test_result->failed_assertion);
        }

        printf(COLOR_BOLD(RED, " failed."));
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

    if (test_result->assertion_line != test->line_number || strcmp(test_result->assertion_file, test->filename) != 0)
    {
        printf("\n");
        display_assertion(test_result->assertion_file, test_result->assertion_line);
    }

    if (unicorn_test_result_has_diff(test_result))
    {
        printf("\n");
        display_diff(test_result->diff_original, test_result->diff_modified);
    }

    if (test_result->output_length > 0)
    {
        printf("\n");
        display_output(test_result->output_buffer);
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
 * Progress utils
 */

static void display_dot_string(UnicornSessionOutputState *output_state)
{
    char string_label[64];

    snprintf(string_label, sizeof (string_label), COLOR(MAGENTA, "%d") " - " COLOR(MAGENTA, "%d"), output_state->index, output_state->index + output_state->length - 1);
    printf("%28s |  ", string_label);

    for (int i = 0; i < output_state->length; i++)
    {
        printf(output_state->string[i] == '.' ? "." : COLOR_BOLD(RED, "F"));
    }

    fflush(stdout);
}


/*
 * Public output functions
 */

void unicorn_output_string(FILE* stream, char *string, size_t line_number, char *indent)
{
    char *pos = strchr(string, '\n');
    char *line = string;

    while (pos != NULL)
    {
        fprintf(stream, "%s", indent);

        if (line_number > 0) {
            fprintf(stream, COLOR(MAGENTA, "%6zu"), line_number);
            fprintf(stream, " |  ");
        }

        fprintf(stream, "%.*s\n", (int)(pos - line), line);

        line = pos + 1;
        pos = strchr(line, '\n');

        if (line_number > 0) {
            line_number++;
        }
    }

    fprintf(stream, "%s", indent);

    if (line_number > 0) {
        fprintf(stream, COLOR(MAGENTA, "%6zu"), line_number);
        fprintf(stream, " |  ");
    }

    fprintf(stream, "%s\n", line);
}

void unicorn_output_session_init(UnicornTestSession *test_session)
{
    printf("\nRunning tests...\n");

    UnicornSessionOutputState *output_state = &test_session->output_state;

    output_state->length = sizeof (output_state->string);
    output_state->index = -output_state->length + 1;
}

void unicorn_output_session_progress(UnicornTestSession *test_session)
{
    UnicornSessionOutputState *output_state = &test_session->output_state;
    UnicornTestResult *last_result = test_session->results->last->value;

    if (output_state->length < (int)sizeof (output_state->string))
    {
        output_state->string[output_state->length] = last_result->success ? '.' : 'F';
        output_state->length++;

        printf("\r");
        display_dot_string(&test_session->output_state);
    }
    else
    {
        output_state->index += sizeof (output_state->string);
        output_state->length = 1;

        output_state->string[0] = last_result->success ? '.' : 'F';

        printf("\n");
        display_dot_string(&test_session->output_state);
    }
}

void unicorn_output_session_result(UnicornTestSession *test_session)
{
    if (test_session->results->count > 0)
    {
        printf("\n");
        display_results(test_session);
    }

    if (test_session->failures->count > 0)
    {
        display_failing_tests(test_session);
    }

    display_session_summary(test_session);
}
