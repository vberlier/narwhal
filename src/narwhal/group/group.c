#include "narwhal/group/group.h"

#include <stdlib.h>

#include "narwhal/collection/collection.h"
#include "narwhal/test/test.h"

/*
 * Group creation
 */

static void initialize_test_group(NarwhalTestGroup *test_group,
                                  const char *name,
                                  NarwhalGroupItemRegistration *group_items,
                                  size_t item_count)
{
    test_group->name = name;
    test_group->group = NULL;
    test_group->subgroups = narwhal_empty_collection();
    test_group->tests = narwhal_empty_collection();

    for (size_t i = 0; i < item_count; i++)
    {
        NarwhalGroupItemRegistration registration = group_items[i];
        registration(test_group);
    }
}

NarwhalTestGroup *narwhal_new_test_group(const char *name,
                                         NarwhalGroupItemRegistration *group_items,
                                         size_t item_count)
{
    NarwhalTestGroup *test_group = malloc(sizeof(NarwhalTestGroup));
    initialize_test_group(test_group, name, group_items, item_count);

    return test_group;
}

/*
 * Group items registration
 */

void narwhal_register_subgroup(NarwhalTestGroup *test_group,
                               const char *name,
                               NarwhalGroupItemRegistration *group_items,
                               size_t item_count)
{
    NarwhalTestGroup *subgroup = narwhal_new_test_group(name, group_items, item_count);
    subgroup->group = test_group;

    narwhal_collection_append(test_group->subgroups, subgroup);
}

void narwhal_register_test(NarwhalTestGroup *test_group,
                           const char *name,
                           const char *filename,
                           size_t line_number,
                           NarwhalTestFunction function,
                           NarwhalTestModifierRegistration *test_modifiers,
                           size_t modifier_count)
{
    NarwhalTest *test =
        narwhal_new_test(name, filename, line_number, function, test_modifiers, modifier_count);
    test->group = test_group;

    narwhal_collection_append(test_group->tests, test);
}

/*
 * Cleanup
 */

void narwhal_free_test_group(NarwhalTestGroup *test_group)
{
    while (test_group->subgroups->count > 0)
    {
        NarwhalTestGroup *subgroup = narwhal_collection_pop(test_group->subgroups);
        narwhal_free_test_group(subgroup);
    }
    narwhal_free_collection(test_group->subgroups);

    while (test_group->tests->count > 0)
    {
        NarwhalTest *test = narwhal_collection_pop(test_group->tests);
        narwhal_free_test(test);
    }
    narwhal_free_collection(test_group->tests);

    free(test_group);
}
