#ifndef UNICORN_TEST_H
#define UNICORN_TEST_H


#include <stdlib.h>

#include "unicorn/types.h"


struct UnicornTest
{
    char *name;
    char *filename;
    size_t line_number;
    UnicornTestGroup *group;
    UnicornTestFunction function;
    UnicornCollection *resources;
    UnicornCollection *fixtures;
    UnicornCollection *params;
    UnicornCollection *accessible_fixtures;
    UnicornCollection *accessible_params;
    UnicornTestResult *result;
};

UnicornTest *unicorn_new_test(char *name, char *filename, size_t line_number, UnicornTestFunction function, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);
void unicorn_run_test(UnicornTest *test);

void unicorn_free_after_test(UnicornTest *test, void *resource);
void *unicorn_test_resource(UnicornTest *test, size_t size);
void unicorn_free_test_resources(UnicornTest *test);

void unicorn_register_test_fixture(UnicornTest *test, UnicornCollection *access_collection, char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);
void unicorn_register_test_param(UnicornTest *test, UnicornCollection *access_collection, char *name, void *values, size_t count);

void unicorn_free_test(UnicornTest *test);


#define DECLARE_TEST(test_name) \
    extern UnicornTestModifierRegistration _unicorn_test_modifiers_ ## test_name[]; \
    void _unicorn_test_function_ ## test_name(UNUSED UnicornTest *_unicorn_test, UNUSED UnicornCollection *_unicorn_params, UNUSED UnicornCollection *_unicorn_fixtures); \
    void test_name(UnicornTestGroup *test_group)


#define TEST(test_name, ...) \
    DECLARE_TEST(test_name); \
    UnicornTestModifierRegistration _unicorn_test_modifiers_ ## test_name[] = { __VA_ARGS__ }; \
    void test_name(UnicornTestGroup *test_group) \
    { \
        unicorn_register_test(test_group, #test_name, __FILE__, __LINE__, _unicorn_test_function_ ## test_name, _unicorn_test_modifiers_ ## test_name, sizeof (_unicorn_test_modifiers_ ## test_name) / sizeof (*_unicorn_test_modifiers_ ## test_name)); \
    } \
    void _unicorn_test_function_ ## test_name(UNUSED UnicornTest *_unicorn_test, UNUSED UnicornCollection *_unicorn_params, UNUSED UnicornCollection *_unicorn_fixtures)


#endif
