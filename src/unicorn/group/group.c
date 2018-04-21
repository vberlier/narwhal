#include <stdlib.h>

#include "unicorn/collection/collection.h"
#include "unicorn/group/group.h"
#include "unicorn/test/test.h"


/*
 * Group creation
 */

static void initialize_test_group(UnicornTestGroup *test_group, char *name, UnicornGroupItemRegistration *group_items, size_t item_count)
{
    test_group->name = name;
    test_group->group = NULL;
    test_group->subgroups = unicorn_empty_collection();
    test_group->tests = unicorn_empty_collection();

    for (size_t i = 0; i < item_count; i++)
    {
        UnicornGroupItemRegistration registration = group_items[i];
        registration(test_group);
    }
}

UnicornTestGroup *unicorn_new_test_group(char *name, UnicornGroupItemRegistration *group_items, size_t item_count)
{
    UnicornTestGroup *test_group = malloc(sizeof (UnicornTestGroup));
    initialize_test_group(test_group, name, group_items, item_count);

    return test_group;
}


/*
 * Group items registration
 */

void unicorn_register_subgroup(UnicornTestGroup *test_group, char *name, UnicornGroupItemRegistration *group_items, size_t item_count)
{
    UnicornTestGroup *subgroup = unicorn_new_test_group(name, group_items, item_count);
    subgroup->group = test_group;

    unicorn_collection_append(test_group->subgroups, subgroup);
}

void unicorn_register_test(UnicornTestGroup *test_group, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    UnicornTest *test = unicorn_new_test(name, filename, line_number, function, test_modifiers, modifier_count);
    test->group = test_group;

    unicorn_collection_append(test_group->tests, test);
}


/*
 * Cleanup
 */

void unicorn_free_test_group(UnicornTestGroup *test_group)
{
    while (test_group->subgroups->count > 0)
    {
        UnicornTestGroup *subgroup = unicorn_collection_pop(test_group->subgroups);
        unicorn_free_test_group(subgroup);
    }
    unicorn_free_collection(test_group->subgroups);

    while (test_group->tests->count > 0)
    {
        UnicornTest *test = unicorn_collection_pop(test_group->tests);
        unicorn_free_test(test);
    }
    unicorn_free_collection(test_group->tests);

    free(test_group);
}
