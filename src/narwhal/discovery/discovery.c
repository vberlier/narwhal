#include "narwhal/discovery/discovery.h"

#include <stddef.h>

#include "narwhal/group/group.h"

/*
 * Global test discovery queue
 */

static NarwhalTestDiscoveryQueue *_narwhal_global_test_discovery_queue = NULL;

/*
 * Register test
 */

void narwhal_register_test_for_discovery(NarwhalTestDiscoveryQueue *test_entry)
{
    narwhal_add_test_to_discovery_queue(test_entry, &_narwhal_global_test_discovery_queue);
}

void narwhal_add_test_to_discovery_queue(NarwhalTestDiscoveryQueue *test_entry,
                                         NarwhalTestDiscoveryQueue **queue)
{
    if (*queue == NULL)
    {
        *queue = test_entry;
        test_entry->next = test_entry;
    }
    else
    {
        NarwhalTestDiscoveryQueue *previous = *queue;
        test_entry->next = previous->next;
        previous->next = test_entry;
        *queue = test_entry;
    }
}

/*
 * Create test group from discovery queue
 */

NarwhalTestGroup *narwhal_discover_tests()
{
    return narwhal_collect_tests_from_discovery_queue("root",
                                                      _narwhal_global_test_discovery_queue);
}

NarwhalTestGroup *narwhal_collect_tests_from_discovery_queue(const char *group_name,
                                                             NarwhalTestDiscoveryQueue *queue)
{
    NarwhalTestGroup *test_group = narwhal_new_test_group(group_name, NULL, 0);

    if (queue != NULL)
    {
        NarwhalTestDiscoveryQueue *entry = queue;

        do
        {
            entry = entry->next;
            entry->test_handle(test_group);
        } while (entry != queue);
    }

    return test_group;
}
