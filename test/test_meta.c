#include "unicorn/unicorn.h"


/*
 * Meta tests run inside of another test
 */

TEST(meta_empty)
{

}

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


/*
 * Define the meta test parameter
 */

TEST_PARAM(meta_test, struct { UnicornGroupItemRegistration handle; char *error; },
{
    { meta_empty, .error = NULL },
    { meta_failing, .error = "No details available." },
    { meta_failing_with_message, .error = "Something went wrong." },
    { meta_failing_with_formatted_message, .error = "Invalid result 42." },
    { meta_assertion, .error = NULL },
    { meta_assertion_with_message, .error = NULL },
    { meta_assertion_with_formatted_message, .error = NULL },
    { meta_failing_assertion, .error = "No details available." },
    { meta_failing_assertion_with_message, .error = "The value should be 42." },
    { meta_failing_assertion_with_formatted_message, .error = "The value is -1 but it should be 42." },
    { meta_equality, .error = NULL },
    { meta_failing_equality, .error = "First argument is equal to -1 instead of 42." },
    { meta_string_equality, .error = NULL },
    { meta_failing_string_equality, .error = "First argument is equal to \"-1\" instead of \"42\"." },
    { meta_substring, .error = NULL },
    { meta_failing_substring, .error = "First argument is equal to \"Wrong value!\" and doesn't contain \"world\"." }
})


/*
 * Test sample meta tests
 */

TEST(run_meta_test, meta_test)
{
    GET_PARAM(meta_test);

    int status_code = -1;

    CAPTURE_OUTPUT(test_output)
    {
        status_code = RUN_TESTS(meta_test.handle);
    }

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
