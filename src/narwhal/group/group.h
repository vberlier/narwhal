#ifndef NARWHAL_GROUP_H
#define NARWHAL_GROUP_H

#include <stdbool.h>
#include <stdlib.h>

#include "narwhal/types.h"
#include "narwhal/unused_attribute.h"

struct NarwhalTestGroup
{
    const char *name;
    bool only;
    NarwhalTestGroup *group;
    NarwhalCollection *subgroups;
    NarwhalCollection *tests;
};

NarwhalTestGroup *narwhal_new_test_group(const char *name,
                                         NarwhalGroupItemRegistration *group_items,
                                         size_t item_count);
void narwhal_register_subgroup(NarwhalTestGroup *test_group,
                               const char *name,
                               NarwhalGroupItemRegistration *group_items,
                               size_t item_count);
void narwhal_register_test(NarwhalTestGroup *test_group,
                           const char *name,
                           const char *filename,
                           size_t line_number,
                           NarwhalTestFunction function,
                           NarwhalTestModifierRegistration *test_modifiers,
                           size_t modifier_count);

void narwhal_free_test_group(NarwhalTestGroup *test_group);

#define DECLARE_GROUP(group_name) void group_name(NarwhalTestGroup *test_group)

#define TEST_GROUP(group_name, ...)                                                        \
    DECLARE_GROUP(group_name);                                                             \
    static NarwhalGroupItemRegistration _narwhal_group_items_##group_name[] = __VA_ARGS__; \
    void group_name(NarwhalTestGroup *test_group)                                          \
    {                                                                                      \
        narwhal_register_subgroup(test_group,                                              \
                                  #group_name,                                             \
                                  _narwhal_group_items_##group_name,                       \
                                  sizeof(_narwhal_group_items_##group_name) /              \
                                      sizeof(*_narwhal_group_items_##group_name));         \
    }

#endif
