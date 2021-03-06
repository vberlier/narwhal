#ifndef NARWHAL_DIFF_H
#define NARWHAL_DIFF_H

#include <stdlib.h>

#include "narwhal/types.h"

struct NarwhalDiffMatrix
{
    size_t rows;
    size_t columns;
    int *content;
};

enum NarwhalDiffChunkType
{
    NARWHAL_DIFF_CHUNK_TYPE_MATCHED,
    NARWHAL_DIFF_CHUNK_TYPE_ADDED,
    NARWHAL_DIFF_CHUNK_TYPE_REPLACED,
    NARWHAL_DIFF_CHUNK_TYPE_DELETED
};

struct NarwhalDiff
{
    size_t size;
    NarwhalDiffChunk *chunks;
};

struct NarwhalDiffChunk
{
    NarwhalDiffChunkType type;
    size_t original_start;
    size_t original_end;
    size_t modified_start;
    size_t modified_end;
};

NarwhalDiffMatrix *narwhal_new_diff_matrix(size_t rows, size_t columns);
NarwhalDiffMatrix *narwhal_new_diff_matrix_from_lengths(size_t original_length,
                                                        size_t modified_lengths);
void narwhal_diff_matrix_fill_from_strings(NarwhalDiffMatrix *diff_matrix,
                                           const char *original,
                                           const char *modified);
void narwhal_diff_matrix_fill_from_lines(NarwhalDiffMatrix *diff_matrix,
                                         const char *original,
                                         const char *modified);
NarwhalDiff narwhal_diff_matrix_get_diff(const NarwhalDiffMatrix *diff_matrix);

size_t narwhal_diff_matrix_index(const NarwhalDiffMatrix *diff_matrix, size_t row, size_t column);
int narwhal_diff_matrix_get(const NarwhalDiffMatrix *diff_matrix, size_t row, size_t column);
void narwhal_diff_matrix_set(const NarwhalDiffMatrix *diff_matrix,
                             size_t row,
                             size_t column,
                             int value);

NarwhalDiff narwhal_diff_strings_lengths(const char *original,
                                         size_t original_length,
                                         const char *modified,
                                         size_t modified_length);
NarwhalDiff narwhal_diff_strings(const char *original, const char *modified);
NarwhalDiff narwhal_diff_lines(const char *original, const char *modified);

void narwhal_free_diff_matrix(NarwhalDiffMatrix *diff_matrix);

#endif
