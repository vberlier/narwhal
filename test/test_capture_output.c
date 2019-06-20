#include "narwhal/narwhal.h"


TEST(capture_output_empty)
{
    CAPTURE_OUTPUT(output) { }

    ASSERT_EQ(output, "");
}


TEST(capture_output_simple)
{
    CAPTURE_OUTPUT(output)
    {
        printf("Hello, world!\n");
    }

    ASSERT_EQ(output, "Hello, world!\n");
}


TEST(capture_output_stderr)
{
    CAPTURE_OUTPUT(output)
    {
        fprintf(stdout, "foo\n");
        fprintf(stderr, "bar\n");
    }

    ASSERT_EQ(output, "foo\nbar\n");
}


TEST(capture_output_loop)
{
    CAPTURE_OUTPUT(output)
    {
        for (int i = 0; i < 10; i++)
        {
            printf("%d", i);
        }

        printf("\n");
    }

    ASSERT_EQ(output, "0123456789\n");
}


TEST(meta_capture_output_transparent)
{
    printf("before");

    CAPTURE_OUTPUT(output)
    {
        printf(" inside");
    }

    printf(" after\n");

    FAIL();
}


TEST(meta_capture_output_transparent_interrupt)
{
    printf("before");

    CAPTURE_OUTPUT(output)
    {
        printf(" inside");
        FAIL();
    }

    printf(" after\n");
}


TEST_PARAM(meta_output_capture_test, struct { NarwhalGroupItemRegistration handle; char *output; },
{
    { meta_capture_output_transparent, "before inside after\n" },
    { meta_capture_output_transparent_interrupt, "before inside" }
})


TEST(run_meta_output_capture_test, meta_output_capture_test)
{
    GET_PARAM(meta_output_capture_test);

    int status_code = -1;

    CAPTURE_OUTPUT(test_output)
    {
        status_code = RUN_TESTS(meta_output_capture_test.handle);
    }

    ASSERT_EQ(status_code, EXIT_FAILURE);
    ASSERT_SUBSTRING(test_output, meta_output_capture_test.output);
}


TEST_GROUP(capture_output_tests,
{
    capture_output_empty,
    capture_output_simple,
    capture_output_stderr,
    capture_output_loop,
    run_meta_output_capture_test
})
