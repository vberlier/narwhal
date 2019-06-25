#include "narwhal/narwhal.h"

#include <stdlib.h>

/*
 * Main run function
 */

int narwhal_run_root_group(NarwhalGroupItemRegistration *root_items, size_t item_count)
{
    NarwhalTestGroup *root_group = narwhal_new_test_group("root", root_items, item_count);
    NarwhalTestSession *test_session = narwhal_new_test_session();

    narwhal_test_session_start(test_session);
    narwhal_test_session_run_test_group(test_session, root_group);
    narwhal_test_session_end(test_session);

    int status = test_session->failures->count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

    narwhal_free_test_group(root_group);
    narwhal_free_test_session(test_session);

    return status;
}
