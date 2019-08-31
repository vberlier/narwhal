#ifndef NARWHAL_TEST_H
#define NARWHAL_TEST_H

#include <stdlib.h>

#include "narwhal/types.h"

extern NarwhalTest *_narwhal_current_test;

struct NarwhalTest
{
    const char *name;
    const char *filename;
    size_t line_number;
    NarwhalTestGroup *group;
    NarwhalTestFunction function;
    NarwhalCollection *resources;
    NarwhalCollection *fixtures;
    NarwhalCollection *params;
    NarwhalCollection *accessible_fixtures;
    NarwhalCollection *accessible_params;
    NarwhalTestResult *result;
    NarwhalOutputCapture *output_capture;
};

NarwhalTest *narwhal_new_test(const char *name,
                              const char *filename,
                              size_t line_number,
                              NarwhalTestFunction function,
                              NarwhalTestModifierRegistration *test_modifiers,
                              size_t modifier_count);
void narwhal_run_test(NarwhalTest *test);

void narwhal_free_after_test(NarwhalTest *test, void *resource);
void auto_free(void *resource);
void *narwhal_test_resource(NarwhalTest *test, size_t size);
void *test_resource(size_t size);
void narwhal_free_test_resources(NarwhalTest *test);

void narwhal_register_test_fixture(NarwhalTest *test,
                                   NarwhalCollection *access_collection,
                                   const char *name,
                                   size_t fixture_size,
                                   NarwhalTestFixtureSetup setup,
                                   NarwhalTestModifierRegistration *test_modifiers,
                                   size_t modifier_count);
void narwhal_register_test_param(NarwhalTest *test,
                                 NarwhalCollection *access_collection,
                                 const char *name,
                                 const void *values,
                                 size_t count);

void narwhal_free_test(NarwhalTest *test);

#define DECLARE_TEST(test_name)                                                   \
    extern NarwhalTestModifierRegistration _narwhal_test_modifiers_##test_name[]; \
    void _narwhal_test_function_##test_name();                                    \
    void test_name(NarwhalTestGroup *test_group)

#define TEST(test_name, ...)                                                                 \
    DECLARE_TEST(test_name);                                                                 \
    NarwhalTestModifierRegistration _narwhal_test_modifiers_##test_name[] = { __VA_ARGS__ }; \
    void test_name(NarwhalTestGroup *test_group)                                             \
    {                                                                                        \
        narwhal_register_test(test_group,                                                    \
                              #test_name,                                                    \
                              __FILE__,                                                      \
                              __LINE__,                                                      \
                              _narwhal_test_function_##test_name,                            \
                              _narwhal_test_modifiers_##test_name,                           \
                              sizeof(_narwhal_test_modifiers_##test_name) /                  \
                                  sizeof(*_narwhal_test_modifiers_##test_name));             \
    }                                                                                        \
    void _narwhal_test_function_##test_name()

#endif
