#include "narwhal/narwhal.h"

TEST(meta_group_test_passing_1) {}
TEST(meta_group_test_passing_2) {}
TEST(meta_group_test_passing_3) {}
TEST(meta_group_test_passing_4) {}

TEST(meta_group_test_failing_1)
{
    FAIL();
}
TEST(meta_group_test_failing_2)
{
    FAIL();
}
TEST(meta_group_test_failing_3)
{
    FAIL();
}
TEST(meta_group_test_failing_4)
{
    FAIL();
}

/*
 * Meta groups run inside of a test
 */

TEST_GROUP(meta_empty_group,
           {

           })

TEST_GROUP(meta_single_passing_group, { meta_group_test_passing_1 })

TEST_GROUP(meta_single_failing_group, { meta_group_test_failing_1 })

TEST_GROUP(meta_double_passing_group, { meta_group_test_passing_1, meta_group_test_passing_2 })

TEST_GROUP(meta_double_failing_group, { meta_group_test_failing_1, meta_group_test_failing_2 })

TEST_GROUP(meta_double_mixed_group, { meta_group_test_passing_1, meta_group_test_failing_1 })

TEST_GROUP(meta_bigger_passing_group,
           { meta_group_test_passing_1,
             meta_group_test_passing_2,
             meta_group_test_passing_3,
             meta_group_test_passing_4 })

TEST_GROUP(meta_bigger_failing_group,
           { meta_group_test_failing_1,
             meta_group_test_failing_2,
             meta_group_test_failing_3,
             meta_group_test_failing_4 })

TEST_GROUP(meta_bigger_mixed_group,
           { meta_group_test_passing_1,
             meta_group_test_failing_1,
             meta_group_test_passing_2,
             meta_group_test_failing_2,
             meta_group_test_passing_3,
             meta_group_test_failing_3,
             meta_group_test_passing_4,
             meta_group_test_failing_4 })

/*
 * Define the meta group parameter
 */

struct MetaGroup
{
    NarwhalGroupItemRegistration handle;
    char *passing;
    char *failing;
    char *total;
};

TEST_PARAM(meta_group,
           struct MetaGroup,
           { { meta_empty_group, "0 passed", NULL, "0 total" },
             { meta_single_passing_group, "1 passed", NULL, "1 total" },
             { meta_single_failing_group, "0 passed", "1 failed", "1 total" },
             { meta_double_passing_group, "2 passed", NULL, "2 total" },
             { meta_double_failing_group, "0 passed", "2 failed", "2 total" },
             { meta_double_mixed_group, "1 passed", "1 failed", "2 total" },
             { meta_bigger_passing_group, "4 passed", NULL, "4 total" },
             { meta_bigger_failing_group, "0 passed", "4 failed", "4 total" },
             { meta_bigger_mixed_group, "4 passed", "4 failed", "8 total" } })

/*
 * Run sample meta groups
 */

TEST(run_meta_group, meta_group)
{
    GET_PARAM(meta_group);

    int status_code = -1;

    CAPTURE_OUTPUT(test_output) { status_code = RUN_TESTS(meta_group.handle); }

    ASSERT_SUBSTRING(test_output, meta_group.passing);
    ASSERT_SUBSTRING(test_output, meta_group.total);

    if (meta_group.failing != NULL)
    {
        ASSERT_EQ(status_code, EXIT_FAILURE);
        ASSERT_SUBSTRING(test_output, meta_group.failing);
    }
    else
    {
        ASSERT_EQ(status_code, EXIT_SUCCESS);
    }
}
