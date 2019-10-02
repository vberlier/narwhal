#include "narwhal/narwhal.h"

#ifdef __GNUC__

__attribute__((weak)) int main(void)
{
    NarwhalTestGroup *root_group = narwhal_discover_tests();

    int status = narwhal_run_root_group(root_group);

    narwhal_free_test_group(root_group);

    return status;
}

#endif
