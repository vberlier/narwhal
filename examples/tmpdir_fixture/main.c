#include <stdio.h>

#include "narwhal.h"
#include "tmpdir.h"

TEST(example, tmpdir)
{
    FILE *data = fopen("data.txt", "w");

    fputs("Hello, world!\n", data);
    fclose(data);

    data = fopen("data.txt", "r");

    char buff[32];
    fgets(buff, sizeof(buff), data);
    fclose(data);

    ASSERT_EQ((char *)buff, "Hello, world!\n");
}
