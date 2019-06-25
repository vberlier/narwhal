#include "narwhal/narwhal.h"

/*
 * Meta tests run inside of another test
 */

TEST(meta_empty) {}

TEST(meta_failing)
{
    FAIL();
}

TEST(meta_failing_with_message)
{
    FAIL("Something went wrong.");
}

TEST(meta_failing_with_formatted_message)
{
    int value = 42;
    FAIL("Invalid result %d.", value);
}

TEST(meta_assertion)
{
    int value = 42;
    ASSERT(value == 42);
}

TEST(meta_assertion_with_message)
{
    int value = 42;
    ASSERT(value == 42, "The value should be 42.");
}

TEST(meta_assertion_with_formatted_message)
{
    int value = 42;
    ASSERT(value == 42, "The value is %d but it should be 42.", value);
}

TEST(meta_failing_assertion)
{
    int value = -1;
    ASSERT(value == 42);
}

TEST(meta_failing_assertion_with_message)
{
    int value = -1;
    ASSERT(value == 42, "The value should be 42.");
}

TEST(meta_failing_assertion_with_formatted_message)
{
    int value = -1;
    ASSERT(value == 42, "The value is %d but it should be 42.", value);
}

TEST(meta_equality)
{
    int value = 42;
    ASSERT_EQ(value, 42);
}

TEST(meta_string_equality)
{
    char *value = "42";
    ASSERT_EQ(value, "42");
}

TEST(meta_failing_equality)
{
    int value = -1;
    ASSERT_EQ(value, 42);
}

TEST(meta_failing_string_equality)
{
    char *value = "-1";
    ASSERT_EQ(value, "42");
}

TEST(meta_inequality)
{
    int value = -1;
    ASSERT_NE(value, 42);
}

TEST(meta_string_inequality)
{
    char *value = "-1";
    ASSERT_NE(value, "42");
}

TEST(meta_failing_inequality)
{
    int value = 42;
    ASSERT_NE(value, 42);
}

TEST(meta_failing_string_inequality)
{
    char *value = "42";
    ASSERT_NE(value, "42");
}

TEST(meta_less_than)
{
    int value = 1;
    ASSERT_LT(value, 2);
}

TEST(meta_failing_less_than)
{
    int value = 7;
    ASSERT_LT(value, 2);
}

TEST(meta_less_equal)
{
    int value = 1;
    ASSERT_LE(value, value);
    ASSERT_LE(value, 2);
}

TEST(meta_failing_less_equal)
{
    int value = 7;
    ASSERT_LE(value, value);
    ASSERT_LE(value, 2);
}

TEST(meta_greater_than)
{
    int value = 1;
    ASSERT_GT(value, -2);
}

TEST(meta_failing_greater_than)
{
    int value = -7;
    ASSERT_GT(value, -2);
}

TEST(meta_greater_equal)
{
    int value = 1;
    ASSERT_GE(value, value);
    ASSERT_GE(value, -2);
}

TEST(meta_failing_greater_equal)
{
    int value = -7;
    ASSERT_GE(value, value);
    ASSERT_GE(value, -2);
}

TEST(meta_substring)
{
    char *message = "Hello, world!";
    ASSERT_SUBSTRING(message, "world");
}

TEST(meta_failing_substring)
{
    char *message = "Wrong value!";
    ASSERT_SUBSTRING(message, "world");
}

TEST(meta_not_substring)
{
    char *message = "Wrong value!";
    ASSERT_NOT_SUBSTRING(message, "world");
}

TEST(meta_failing_not_substring)
{
    char *message = "Hello, world!";
    ASSERT_NOT_SUBSTRING(message, "world");
}

TEST(meta_exit_success)
{
    exit(EXIT_SUCCESS);
}

TEST(meta_exit_failure)
{
    exit(EXIT_FAILURE);
}

TEST(meta_segfault)
{
    int *boom = NULL;
    *boom = 42;
}

/*
 * Define the meta test parameter
 */

TEST_PARAM(
    meta_test,
    struct {
        NarwhalGroupItemRegistration handle;
        char *error;
    },
    { { meta_empty, .error = NULL },
      { meta_failing, .error = "No details available." },
      { meta_failing_with_message, .error = "Something went wrong." },
      { meta_failing_with_formatted_message, .error = "Invalid result 42." },
      { meta_assertion, .error = NULL },
      { meta_assertion_with_message, .error = NULL },
      { meta_assertion_with_formatted_message, .error = NULL },
      { meta_failing_assertion, .error = "No details available." },
      { meta_failing_assertion_with_message, .error = "The value should be 42." },
      { meta_failing_assertion_with_formatted_message,
        .error = "The value is -1 but it should be 42." },

      { meta_equality, .error = NULL },
      { meta_failing_equality, .error = "First argument -1 is not equal to 42." },
      { meta_string_equality, .error = NULL },
      { meta_failing_string_equality, .error = "See diff for details." },
      { meta_inequality, .error = NULL },
      { meta_failing_inequality, .error = "First argument 42 is not different from 42." },
      { meta_string_inequality, .error = NULL },
      { meta_failing_string_inequality,
        .error = "First argument \"42\" is not different from \"42\"." },
      { meta_less_than, .error = NULL },
      { meta_failing_less_than, .error = "First argument 7 is not less than 2." },
      { meta_less_equal, .error = NULL },
      { meta_failing_less_equal, .error = "First argument 7 is not less than or equal to 2." },
      { meta_greater_than, .error = NULL },
      { meta_failing_greater_than, .error = "First argument -7 is not greater than -2." },
      { meta_greater_equal, .error = NULL },
      { meta_failing_greater_equal, .error = "First argument -7 is not greater or equal to -2." },
      { meta_substring, .error = NULL },
      { meta_failing_substring,
        .error = "First argument \"Wrong value!\" doesn't contain \"world\"." },
      { meta_not_substring, .error = NULL },
      { meta_failing_not_substring,
        .error = "First argument \"Hello, world!\" contains \"world\"." },

      { meta_exit_success, .error = "Test process exited unexpectedly." },
      { meta_exit_failure, .error = "Test process exited unexpectedly." },
      { meta_segfault, .error = "Test process exited unexpectedly." } })

/*
 * Test sample meta tests
 */

TEST(run_meta_test, meta_test)
{
    GET_PARAM(meta_test);

    int status_code = -1;

    CAPTURE_OUTPUT(test_output) { status_code = RUN_TESTS(meta_test.handle); }

    if (meta_test.error == NULL)
    {
        ASSERT_EQ(status_code, EXIT_SUCCESS);
    }
    else
    {
        ASSERT_EQ(status_code, EXIT_FAILURE);
        ASSERT_SUBSTRING(test_output, meta_test.error);
    }
}
