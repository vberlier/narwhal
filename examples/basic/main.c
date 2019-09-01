#include "narwhal.h"

TEST(example)
{
    ASSERT_EQ("actual", "expected");
}

int main(void)
{
    return RUN_TESTS(example);
}
