#ifndef UNICORN_DIFF_H
#define UNICORN_DIFF_H


#include "unicorn/types.h"


struct UnicornDiffMatrix
{
    size_t rows;
    size_t columns;
    int *content;
};

enum UnicornDiffChunkType
{
    UNICORN_DIFF_CHUNK_TYPE_MATCHED,
    UNICORN_DIFF_CHUNK_TYPE_ADDED,
    UNICORN_DIFF_CHUNK_TYPE_REPLACED,
    UNICORN_DIFF_CHUNK_TYPE_DELETED
};

struct UnicornDiff
{
    size_t size;
    UnicornDiffChunk *chunks;
};

struct UnicornDiffChunk
{
    UnicornDiffChunkType type;
    size_t original_start;
    size_t original_end;
    size_t modified_start;
    size_t modified_end;
};

UnicornDiffMatrix *unicorn_new_diff_matrix(size_t rows, size_t columns);
UnicornDiffMatrix *unicorn_new_diff_matrix_from_lengths(size_t original_length, size_t modified_lengths);
void unicorn_diff_matrix_fill_from_strings(UnicornDiffMatrix *diff_matrix, char *original, char *modified);
void unicorn_diff_matrix_fill_from_lines(UnicornDiffMatrix *diff_matrix, char *original, char *modified);
UnicornDiff unicorn_diff_matrix_get_diff(UnicornDiffMatrix *diff_matrix);

size_t unicorn_diff_matrix_index(UnicornDiffMatrix *diff_matrix, size_t row, size_t column);
int unicorn_diff_matrix_get(UnicornDiffMatrix *diff_matrix, size_t row, size_t column);
void unicorn_diff_matrix_set(UnicornDiffMatrix *diff_matrix, size_t row, size_t column, int value);

UnicornDiff unicorn_diff_strings(char *original, char *modified);
UnicornDiff unicorn_diff_lines(char *original, char *modified);

void unicorn_free_diff_matrix(UnicornDiffMatrix *diff_matrix);


#endif
