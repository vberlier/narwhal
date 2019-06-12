#include "unicorn/unicorn.h"


TEST(diff_matrix_initialization)
{
    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix(16, 42);

    ASSERT_EQ(diff_matrix->rows, (size_t)16);
    ASSERT_EQ(diff_matrix->columns, (size_t)42);

    unicorn_free_diff_matrix(diff_matrix);
}


TEST(diff_matrix_from_lengths)
{
    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix_from_lengths(16, 42);

    ASSERT_EQ(diff_matrix->rows, (size_t)43);
    ASSERT_EQ(diff_matrix->columns, (size_t)17);

    for (size_t i = 0; i < diff_matrix->rows; i++)
    {
        int value = unicorn_diff_matrix_get(diff_matrix, i, 0);
        ASSERT_EQ(value, (int)i);
    }

    for (size_t j = 0; j < diff_matrix->columns; j++)
    {
        int value = unicorn_diff_matrix_get(diff_matrix, 0, j);
        ASSERT_EQ(value, (int)j);
    }

    unicorn_free_diff_matrix(diff_matrix);
}


TEST(diff_matrix_fill_from_strings)
{
    char *original = "abcdef";
    char *modified = "azced";

    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix_from_lengths(strlen(original), strlen(modified));

    ASSERT_EQ(diff_matrix->rows, (size_t)6);
    ASSERT_EQ(diff_matrix->columns, (size_t)7);

    unicorn_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    int min_operations = unicorn_diff_matrix_get(diff_matrix, diff_matrix->rows - 1, diff_matrix->columns - 1);

    ASSERT_EQ(min_operations, 3);

    unicorn_free_diff_matrix(diff_matrix);
}


TEST(diff_matrix_get_diff)
{
    char *original = "abcdef";
    char *modified = "azced";

    UnicornDiffMatrix *diff_matrix = unicorn_new_diff_matrix_from_lengths(strlen(original), strlen(modified));
    unicorn_diff_matrix_fill_from_strings(diff_matrix, original, modified);

    UnicornDiff diff = unicorn_diff_matrix_get_diff(diff_matrix);

    ASSERT_EQ(diff.size, (size_t)6);

    free(diff.chunks);

    unicorn_free_diff_matrix(diff_matrix);
}


TEST_GROUP(diff_tests,
{
    diff_matrix_initialization,
    diff_matrix_from_lengths,
    diff_matrix_fill_from_strings,
    diff_matrix_get_diff
})
