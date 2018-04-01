#ifndef UNICORN_GROUP_H
#define UNICORN_GROUP_H


#include <stdlib.h>

#include "unicorn/types.h"
#include "unicorn/unused_attribute.h"


struct UnicornTestGroup
{
    char *name;
    UnicornTestGroup *group;
    UnicornCollection *subgroups;
    UnicornCollection *tests;
};

UnicornTestGroup *unicorn_new_test_group(char *name, UnicornGroupItemRegistration *group_items, size_t item_count);
void unicorn_register_subgroup(UnicornTestGroup *test_group, char *name, UnicornGroupItemRegistration *group_items, size_t item_count);
void unicorn_register_test(UnicornTestGroup *test_group, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestItemRegistration *test_items, size_t item_count);

void unicorn_free_test_group(UnicornTestGroup *test_group);


#define DECLARE_GROUP(group_name) \
    extern UnicornGroupItemRegistration _unicorn_group_items_ ## group_name[]; \
    void group_name(UnicornTestGroup *test_group)


#define TEST_GROUP(group_name, ...) \
    DECLARE_GROUP(group_name); \
    UnicornGroupItemRegistration _unicorn_group_items_ ## group_name[] = __VA_ARGS__; \
    void group_name(UnicornTestGroup *test_group) \
    { \
        unicorn_register_subgroup(test_group, #group_name, _unicorn_group_items_ ## group_name, sizeof (_unicorn_group_items_ ## group_name) / sizeof (*_unicorn_group_items_ ## group_name)); \
    }


#endif
