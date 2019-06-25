#include "narwhal/diff/diff.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

#include "narwhal/utils.h"

/*
 * Diff matrix initialization
 */

static void initialize_diff_matrix(NarwhalDiffMatrix *diff_matrix, size_t rows, size_t columns)
{
    diff_matrix->rows = rows;
    diff_matrix->columns = columns;
    diff_matrix->content = malloc(rows * columns * sizeof(int));
}

NarwhalDiffMatrix *narwhal_new_diff_matrix(size_t rows, size_t columns)
{
    NarwhalDiffMatrix *diff_matrix = malloc(sizeof(NarwhalDiffMatrix));
    initialize_diff_matrix(diff_matrix, rows, columns);

    return diff_matrix;
}

/*
 * Diff matrix operations
 */

NarwhalDiffMatrix *narwhal_new_diff_matrix_from_lengths(size_t original_length,
                                                        size_t modified_length)
{
    NarwhalDiffMatrix *diff_matrix =
        narwhal_new_diff_matrix(modified_length + 1, original_length + 1);

    for (size_t i = 0; i < diff_matrix->rows; i++)
    {
        narwhal_diff_matrix_set(diff_matrix, i, 0, i);
    }

    for (size_t j = 0; j < diff_matrix->columns; j++)
    {
        narwhal_diff_matrix_set(diff_matrix, 0, j, j);
    }

    return diff_matrix;
}

static void fill_different(NarwhalDiffMatrix *diff_matrix, size_t i, size_t j)
{
    narwhal_diff_matrix_set(
        diff_matrix,
        i,
        j,
        narwhal_min_int(narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1),
                        narwhal_min_int(narwhal_diff_matrix_get(diff_matrix, i - 1, j),
                                        narwhal_diff_matrix_get(diff_matrix, i, j - 1))) +
            1);
}

static void fill_equal(NarwhalDiffMatrix *diff_matrix, size_t i, size_t j)
{
    narwhal_diff_matrix_set(diff_matrix, i, j, narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1));
}

void narwhal_diff_matrix_fill_from_strings(NarwhalDiffMatrix *diff_matrix,
                                           char *original,
                                           char *modified)
{
    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            if (original[j - 1] == modified[i - 1])
            {
                fill_equal(diff_matrix, i, j);
            }
            else
            {
                fill_different(diff_matrix, i, j);
            }
        }
    }
}

void narwhal_diff_matrix_fill_from_lines(NarwhalDiffMatrix *diff_matrix,
                                         char *original,
                                         char *modified)
{
    char *modified_pos;
    char *modified_line = modified;

    for (size_t i = 1; i < diff_matrix->rows; i++)
    {
        modified_pos = narwhal_next_line(modified_line);
        size_t modified_line_length = modified_pos - modified_line;

        char *original_pos;
        char *original_line = original;

        for (size_t j = 1; j < diff_matrix->columns; j++)
        {
            original_pos = narwhal_next_line(original_line);
            size_t original_line_length = original_pos - original_line;

            if (original_line_length == modified_line_length &&
                strncmp(original_line, modified_line, original_line_length) == 0)
            {
                fill_equal(diff_matrix, i, j);
            }
            else
            {
                fill_different(diff_matrix, i, j);
            }

            original_line = original_pos + 1;
        }

        modified_line = modified_pos + 1;
    }
}

NarwhalDiff narwhal_diff_matrix_get_diff(NarwhalDiffMatrix *diff_matrix)
{
    if (diff_matrix->rows == 1 && diff_matrix->columns == 1)
    {
        NarwhalDiff diff = { .size = 0, .chunks = NULL };
        return diff;
    }

    size_t capacity = 32;
    size_t size = 0;
    NarwhalDiffChunk *backtrack = malloc(capacity * sizeof(NarwhalDiffChunk));

    size_t i = diff_matrix->rows - 1;
    size_t j = diff_matrix->columns - 1;

    while (i > 0 || j > 0)
    {
        if (size == capacity)
        {
            capacity *= 2;
            backtrack = realloc(backtrack, capacity * sizeof(NarwhalDiffChunk));
        }

        NarwhalDiffChunk *current_chunk = &backtrack[size];
        size++;

        int current = narwhal_diff_matrix_get(diff_matrix, i, j);

        if (i > 0 && j > 0 && current == narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1) + 1)
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_REPLACED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
        else if (j > 0 && current == narwhal_diff_matrix_get(diff_matrix, i, j - 1) + 1)
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_DELETED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i;
            current_chunk->modified_end = i;
            j--;
        }
        else if (i > 0 && current == narwhal_diff_matrix_get(diff_matrix, i - 1, j) + 1)
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_ADDED;
            current_chunk->original_start = j;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
        }
        else if (i > 0 && j > 0 && current == narwhal_diff_matrix_get(diff_matrix, i - 1, j - 1))
        {
            current_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_MATCHED;
            current_chunk->original_start = j - 1;
            current_chunk->original_end = j;
            current_chunk->modified_start = i - 1;
            current_chunk->modified_end = i;
            i--;
            j--;
        }
    }

    NarwhalDiff diff = { size, malloc(size * sizeof(NarwhalDiffChunk)) };

    ssize_t backtrack_index = size - 1;
    size_t chunk_index = 0;

    diff.chunks[chunk_index] = backtrack[backtrack_index];

    for (backtrack_index--; backtrack_index >= 0; backtrack_index--)
    {
        NarwhalDiffChunk *chunk = &backtrack[backtrack_index];
        NarwhalDiffChunk *previous_chunk = &diff.chunks[chunk_index];

        if (chunk->type == previous_chunk->type)
        {
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else if ((chunk->type == NARWHAL_DIFF_CHUNK_TYPE_REPLACED &&
                  previous_chunk->type != NARWHAL_DIFF_CHUNK_TYPE_MATCHED) ||
                 (chunk->type != NARWHAL_DIFF_CHUNK_TYPE_MATCHED &&
                  previous_chunk->type == NARWHAL_DIFF_CHUNK_TYPE_REPLACED))
        {
            previous_chunk->type = NARWHAL_DIFF_CHUNK_TYPE_REPLACED;
            previous_chunk->original_end = chunk->original_end;
            previous_chunk->modified_end = chunk->modified_end;
        }
        else
        {
            chunk_index++;
            diff.chunks[chunk_index] = *chunk;
        }
    }

    free(backtrack);

    diff.size = chunk_index + 1;
    diff.chunks = realloc(diff.chunks, diff.size * sizeof(NarwhalDiffChunk));

    return diff;
}

size_t narwhal_diff_matrix_index(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return row * diff_matrix->columns + column;
}

int narwhal_diff_matrix_get(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column)
{
    return diff_matrix->content[narwhal_diff_matrix_index(diff_matrix, row, column)];
}

void narwhal_diff_matrix_set(NarwhalDiffMatrix *diff_matrix, size_t row, size_t column, int value)
{
    diff_matrix->content[narwhal_diff_matrix_index(diff_matrix, row, column)] = value;
}

/*
 * Higher-level wrappers
 */

NarwhalDiff narwhal_diff_strings_lengths(char *original,
                                         size_t original_length,
                                         char *modified,
                                         size_t modified_length)
{
    NarwhalDiffMatrix *diff_matrix =
        narwhal_new_diff_matrix_from_lengths(original_length, modified_length);

    narwhal_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    NarwhalDiff diff = narwhal_diff_matrix_get_diff(diff_matrix);

    narwhal_free_diff_matrix(diff_matrix);

    return diff;
}

NarwhalDiff narwhal_diff_strings(char *original, char *modified)
{
    return narwhal_diff_strings_lengths(original, strlen(original), modified, strlen(modified));
}

NarwhalDiff narwhal_diff_lines(char *original, char *modified)
{
    size_t original_length = narwhal_count_chars(original, '\n') + 1;
    size_t modified_length = narwhal_count_chars(modified, '\n') + 1;

    NarwhalDiffMatrix *diff_matrix =
        narwhal_new_diff_matrix_from_lengths(original_length, modified_length);

    narwhal_diff_matrix_fill_from_lines(diff_matrix, original, modified);

    NarwhalDiff diff = narwhal_diff_matrix_get_diff(diff_matrix);

    narwhal_free_diff_matrix(diff_matrix);

    return diff;
}

/*
 * Cleanup
 */

void narwhal_free_diff_matrix(NarwhalDiffMatrix *diff_matrix)
{
    free(diff_matrix->content);
    free(diff_matrix);
}
