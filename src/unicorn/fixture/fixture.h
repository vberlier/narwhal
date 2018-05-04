#ifndef UNICORN_FIXTURE_H
#define UNICORN_FIXTURE_H


#include <stdlib.h>

#include "unicorn/types.h"


struct UnicornTestFixture
{
    char *name;
    size_t size;
    void *value;
    UnicornTestFixtureSetup setup;
    UnicornTestFixtureCleanup cleanup;
    UnicornTest *test;
    UnicornCollection *accessible_fixtures;
    UnicornCollection *accessible_params;
};

UnicornTestFixture *unicorn_new_test_fixture(char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTest *test, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count);
UnicornTestFixture *unicorn_get_test_fixture(UnicornCollection *fixtures, char *fixture_name);
void unicorn_free_test_fixture(UnicornTestFixture *test_fixture);


#define DECLARE_FIXTURE(fixture_name, fixture_type) \
    typedef fixture_type _unicorn_fixture_type_ ## fixture_name; \
    extern UnicornTestModifierRegistration _unicorn_test_fixture_modifiers_ ## fixture_name[]; \
    void _unicorn_fixture_ ## fixture_name ## _setup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture, UNUSED UnicornTest *_unicorn_test, UNUSED UnicornCollection *_unicorn_params, UNUSED UnicornCollection *_unicorn_fixtures); \
    void _unicorn_fixture_ ## fixture_name ## _call_setup(void *value, UnicornTestFixture *test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _cleanup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture, UNUSED UnicornTest *_unicorn_test, UNUSED UnicornCollection *_unicorn_params, UNUSED UnicornCollection *_unicorn_fixtures); \
    void _unicorn_fixture_ ## fixture_name ## _call_cleanup(void *value, UnicornTestFixture *test_fixture); \
    void fixture_name(UnicornTest *test, UnicornCollection *params, UnicornCollection *fixtures) \


#define TEST_FIXTURE(fixture_name, fixture_type, ...) \
    DECLARE_FIXTURE(fixture_name, fixture_type); \
    UnicornTestModifierRegistration _unicorn_test_fixture_modifiers_ ## fixture_name[] = { __VA_ARGS__ }; \
    void _unicorn_fixture_ ## fixture_name ## _call_setup(void *value, UnicornTestFixture *test_fixture) \
    { \
        _unicorn_fixture_ ## fixture_name ## _setup((_unicorn_fixture_type_ ## fixture_name *)value, test_fixture, test_fixture->test, test_fixture->accessible_params, test_fixture->accessible_fixtures); \
    } \
    void fixture_name(UnicornTest *test, UNUSED UnicornCollection *params, UnicornCollection *fixtures) \
    { \
        unicorn_register_test_fixture(test, fixtures, #fixture_name, sizeof (_unicorn_fixture_type_ ## fixture_name), _unicorn_fixture_ ## fixture_name ## _call_setup, _unicorn_test_fixture_modifiers_ ## fixture_name, sizeof (_unicorn_test_fixture_modifiers_ ## fixture_name) / sizeof (*_unicorn_test_fixture_modifiers_ ## fixture_name)); \
    } \
    void _unicorn_fixture_ ## fixture_name ## _setup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture, UNUSED UnicornTest *_unicorn_test, UNUSED UnicornCollection *_unicorn_params, UNUSED UnicornCollection *_unicorn_fixtures)


#define GET_FIXTURE(fixture_name) \
    _unicorn_fixture_type_ ## fixture_name fixture_name = \
    ({ \
        UnicornTestFixture *_unicorn_test_fixture_ ## fixture_name = unicorn_get_test_fixture(_unicorn_fixtures, #fixture_name); \
        if (_unicorn_test_fixture_ ## fixture_name == NULL) FAIL("Fixture \"%s\" hasn't been applied to the test.", #fixture_name); \
        *(_unicorn_fixture_type_ ## fixture_name *)_unicorn_test_fixture_ ## fixture_name->value; \
    })


#define CLEANUP_FIXTURE(fixture_name) \
        _unicorn_test_fixture->cleanup = _unicorn_fixture_ ## fixture_name ## _call_cleanup; \
    } \
    void _unicorn_fixture_ ## fixture_name ## _call_cleanup(void *value, UnicornTestFixture *test_fixture) \
    { \
        _unicorn_fixture_ ## fixture_name ## _cleanup((_unicorn_fixture_type_ ## fixture_name *)value, test_fixture, test_fixture->test, test_fixture->accessible_params, test_fixture->accessible_fixtures); \
    } \
    void _unicorn_fixture_ ## fixture_name ## _cleanup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture, UNUSED UnicornTest *_unicorn_test, UNUSED UnicornCollection *_unicorn_params, UNUSED UnicornCollection *_unicorn_fixtures) \
    {


#endif
