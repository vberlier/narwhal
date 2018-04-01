# Unicorn

> A magical unit testing framework written in C.

```c
#include "unicorn/unicorn.h"

TEST(example)
{
    int result = 2 + 3;
    ASSERT_EQ(result, 5);
}

int main()
{
    return RUN_TESTS(example);
}
```
