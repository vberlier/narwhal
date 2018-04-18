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
    UnicornTestResult *result;
    int output_pipe[2];
    char *output_buffer;
    size_t output_length;
};

UnicornTest *unicorn_new_test(char *name, char *filename, size_t line_number, UnicornTestFunction function, UnicornTestItemRegistration *test_items, size_t item_count);
void unicorn_run_test(UnicornTest *test);

void unicorn_free_after_test(UnicornTest *test, void *resource);
void *unicorn_test_resource(UnicornTest *test, size_t size);
void unicorn_free_test_resources(UnicornTest *test);

void unicorn_register_test_fixture(UnicornTest *test, char *name, size_t fixture_size, UnicornTestFixtureSetup setup);
UnicornTestFixture *unicorn_get_test_fixture(UnicornTest *test, char *fixture_name);

void unicorn_register_test_param(UnicornTest *test, char *name, void *values, size_t count);
UnicornTestParam *unicorn_get_test_param(UnicornTest *test, char *param_name);

void unicorn_free_test(UnicornTest *test);


#define DECLARE_TEST(test_name) \
    extern UnicornTestItemRegistration _unicorn_test_items_ ## test_name[]; \
    void _unicorn_test_function_ ## test_name(UNUSED UnicornTest *test, UNUSED UnicornTest *_unicorn_test); \
    void test_name(UnicornTestGroup *test_group)


#define TEST(test_name, ...) \
    DECLARE_TEST(test_name); \
    UnicornTestItemRegistration _unicorn_test_items_ ## test_name[] = { __VA_ARGS__ }; \
    void test_name(UnicornTestGroup *test_group) \
    { \
        unicorn_register_test(test_group, #test_name, __FILE__, __LINE__, _unicorn_test_function_ ## test_name, _unicorn_test_items_ ## test_name, sizeof (_unicorn_test_items_ ## test_name) / sizeof (*_unicorn_test_items_ ## test_name)); \
    } \
    void _unicorn_test_function_ ## test_name(UNUSED UnicornTest *test, UNUSED UnicornTest *_unicorn_test)


#endif
