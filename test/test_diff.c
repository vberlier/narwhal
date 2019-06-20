#include "narwhal/narwhal.h"


TEST(diff_matrix_initialization)
{
    NarwhalDiffMatrix *diff_matrix = narwhal_new_diff_matrix(16, 42);

    ASSERT_EQ(diff_matrix->rows, (size_t)16);
    ASSERT_EQ(diff_matrix->columns, (size_t)42);

    narwhal_free_diff_matrix(diff_matrix);
}


TEST(diff_matrix_from_lengths)
{
    NarwhalDiffMatrix *diff_matrix = narwhal_new_diff_matrix_from_lengths(16, 42);

    ASSERT_EQ(diff_matrix->rows, (size_t)43);
    ASSERT_EQ(diff_matrix->columns, (size_t)17);

    for (size_t i = 0; i < diff_matrix->rows; i++)
    {
        int value = narwhal_diff_matrix_get(diff_matrix, i, 0);
        ASSERT_EQ(value, (int)i);
    }

    for (size_t j = 0; j < diff_matrix->columns; j++)
    {
        int value = narwhal_diff_matrix_get(diff_matrix, 0, j);
        ASSERT_EQ(value, (int)j);
    }

    narwhal_free_diff_matrix(diff_matrix);
}


TEST(diff_matrix_fill_from_strings)
{
    char *original = "abcdef";
    char *modified = "azced";

    NarwhalDiffMatrix *diff_matrix = narwhal_new_diff_matrix_from_lengths(strlen(original), strlen(modified));

    ASSERT_EQ(diff_matrix->rows, (size_t)6);
    ASSERT_EQ(diff_matrix->columns, (size_t)7);

    narwhal_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    int min_operations = narwhal_diff_matrix_get(diff_matrix, diff_matrix->rows - 1, diff_matrix->columns - 1);

    ASSERT_EQ(min_operations, 3);

    narwhal_free_diff_matrix(diff_matrix);
}


TEST(diff_matrix_get_diff)
{
    char *original = "abcdef";
    char *modified = "azced";

    NarwhalDiffMatrix *diff_matrix = narwhal_new_diff_matrix_from_lengths(strlen(original), strlen(modified));
    narwhal_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    NarwhalDiff diff = narwhal_diff_matrix_get_diff(diff_matrix);

    ASSERT_EQ(diff.size, (size_t)6);

    free(diff.chunks);

    narwhal_free_diff_matrix(diff_matrix);
}


TEST(diff_strings_matched)
{
    NarwhalDiff diff = narwhal_diff_strings("hello", "hello");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)1);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
}


TEST(diff_strings_added)
{
    NarwhalDiff diff = narwhal_diff_strings("hello", "hello world");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)2);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
    ASSERT_EQ(diff.chunks[1].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_ADDED);
    ASSERT_EQ(diff.chunks[1].original_start, diff.chunks[1].original_end);
}


TEST(diff_strings_replaced)
{
    NarwhalDiff diff = narwhal_diff_strings("something", "everything");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)2);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_REPLACED);
    ASSERT_EQ(diff.chunks[1].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
}


TEST(diff_strings_deleted)
{
    NarwhalDiff diff = narwhal_diff_strings("hello world", "hello");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)2);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
    ASSERT_EQ(diff.chunks[1].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_DELETED);
    ASSERT_EQ(diff.chunks[1].modified_start, diff.chunks[1].modified_end);
}


TEST(diff_lines_matched)
{
    NarwhalDiff diff = narwhal_diff_lines("hello\nworld", "hello\nworld");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)1);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
}


TEST(diff_lines_added)
{
    NarwhalDiff diff = narwhal_diff_lines("hello", "hello\nworld\nfoo");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)2);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
    ASSERT_EQ(diff.chunks[1].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_ADDED);
    ASSERT_EQ(diff.chunks[1].original_start, diff.chunks[1].original_end);
}


TEST(diff_lines_replaced)
{
    NarwhalDiff diff = narwhal_diff_lines("some\nthing", "every\nthing");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)2);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_REPLACED);
    ASSERT_EQ(diff.chunks[1].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
}


TEST(diff_lines_deleted)
{
    NarwhalDiff diff = narwhal_diff_lines("hello\nworld", "hello");
    auto_free(diff.chunks);

    ASSERT_EQ(diff.size, (size_t)2);
    ASSERT_EQ(diff.chunks[0].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_MATCHED);
    ASSERT_EQ(diff.chunks[1].type, (NarwhalDiffChunkType)NARWHAL_DIFF_CHUNK_TYPE_DELETED);
    ASSERT_EQ(diff.chunks[1].modified_start, diff.chunks[1].modified_end);
}


TEST_GROUP(diff_tests,
{
    diff_matrix_initialization,
    diff_matrix_from_lengths,
    diff_matrix_fill_from_strings,
    diff_matrix_get_diff,
    diff_strings_matched,
    diff_strings_added,
    diff_strings_replaced,
    diff_strings_deleted,
    diff_lines_matched,
    diff_lines_added,
    diff_lines_replaced,
    diff_lines_deleted
})
