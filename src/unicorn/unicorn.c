#include <stdlib.h>

#include "unicorn/unicorn.h"


/*
 * Main run function
 */

int unicorn_run_root_group(UnicornGroupItemRegistration *root_items, size_t item_count)
{
    UnicornTestGroup *root_group = unicorn_new_test_group("root", root_items, item_count);
    UnicornTestSession *test_session = unicorn_new_test_session();

    unicorn_test_session_start(test_session);
    unicorn_test_session_run_test_group(test_session, root_group);
    unicorn_test_session_end(test_session);

    unicorn_output_session_result(test_session);

    int status = test_session->failures->count == 0 ? EXIT_SUCCESS : EXIT_FAILURE;

    unicorn_free_test_group(root_group);
    unicorn_free_test_session(test_session);

    return status;
}
