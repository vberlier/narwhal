#ifndef NARWHAL_DISCOVERY_H
#define NARWHAL_DISCOVERY_H

#include "narwhal/concat_macro.h"
#include "narwhal/types.h"

struct NarwhalTestDiscoveryQueue
{
    NarwhalGroupItemRegistration test_handle;
    NarwhalTestDiscoveryQueue *next;
};

void narwhal_register_test_for_discovery(NarwhalTestDiscoveryQueue *test_entry);
void narwhal_add_test_to_discovery_queue(NarwhalTestDiscoveryQueue *test_entry,
                                         NarwhalTestDiscoveryQueue **queue);

NarwhalTestGroup *narwhal_discover_tests();
NarwhalTestGroup *narwhal_collect_tests_from_discovery_queue(const char *group_name,
                                                             NarwhalTestDiscoveryQueue *queue);

#ifdef __GNUC__

#define _NARWHAL_REGISTER_TEST_FOR_DISCOVERY(test_name)                     \
    _NARWHAL_CONCAT(_NARWHAL_REGISTER_TEST_FOR_DISCOVERY_WHEN_DISABLED_IS_, \
                    DISABLE_TEST_DISCOVERY)                                 \
    (test_name)

#define _NARWHAL_REGISTER_TEST_FOR_DISCOVERY_WHEN_DISABLED_IS_0(test_name)               \
    __attribute__((constructor)) static void _narwhal_test_constructor_##test_name(void) \
    {                                                                                    \
        static NarwhalTestDiscoveryQueue test_entry = { test_name, NULL };               \
        narwhal_register_test_for_discovery(&test_entry);                                \
    }

#define _NARWHAL_REGISTER_TEST_FOR_DISCOVERY_WHEN_DISABLED_IS_DISABLE_TEST_DISCOVERY(test_name) \
    _NARWHAL_REGISTER_TEST_FOR_DISCOVERY_WHEN_DISABLED_IS_0(test_name)

#define _NARWHAL_REGISTER_TEST_FOR_DISCOVERY_WHEN_DISABLED_IS_1(test_name)
#define _NARWHAL_REGISTER_TEST_FOR_DISCOVERY_WHEN_DISABLED_IS_(test_name)

#else

#define _NARWHAL_REGISTER_TEST_FOR_DISCOVERY(test_name)

#endif

#endif
