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
};

UnicornTestFixture *unicorn_new_test_fixture(char *name, size_t fixture_size, UnicornTestFixtureSetup setup);
void unicorn_free_test_fixture(UnicornTestFixture *test_fixture);


#define DECLARE_FIXTURE(fixture_name, fixture_type) \
    typedef fixture_type _unicorn_fixture_type_ ## fixture_name; \
    void _unicorn_fixture_ ## fixture_name ## _setup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _call_setup(void *value, UnicornTestFixture *test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _cleanup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture); \
    void _unicorn_fixture_ ## fixture_name ## _call_cleanup(void *value, UnicornTestFixture *test_fixture); \
    void fixture_name(UnicornTest *test) \


#define TEST_FIXTURE(fixture_name, fixture_type) \
    DECLARE_FIXTURE(fixture_name, fixture_type); \
    void _unicorn_fixture_ ## fixture_name ## _call_setup(void *value, UnicornTestFixture *test_fixture) \
    { \
        _unicorn_fixture_ ## fixture_name ## _setup((_unicorn_fixture_type_ ## fixture_name *)value, test_fixture); \
    } \
    void fixture_name(UnicornTest *test) \
    { \
        unicorn_register_test_fixture(test, #fixture_name, sizeof (_unicorn_fixture_type_ ## fixture_name), _unicorn_fixture_ ## fixture_name ## _call_setup); \
    } \
    void _unicorn_fixture_ ## fixture_name ## _setup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture)


#define GET_FIXTURE(fixture_name) \
    _unicorn_fixture_type_ ## fixture_name fixture_name = \
    ({ \
        UnicornTestFixture *_unicorn_test_fixture_ ## fixture_name = unicorn_get_test_fixture(test, #fixture_name); \
        *(_unicorn_fixture_type_ ## fixture_name *)_unicorn_test_fixture_ ## fixture_name->value; \
    })


#define CLEANUP_FIXTURE(fixture_name) \
        _unicorn_test_fixture->cleanup = _unicorn_fixture_ ## fixture_name ## _call_cleanup; \
    } \
    void _unicorn_fixture_ ## fixture_name ## _call_cleanup(void *value, UnicornTestFixture *test_fixture) \
    { \
        _unicorn_fixture_ ## fixture_name ## _cleanup((_unicorn_fixture_type_ ## fixture_name *)value, test_fixture); \
    } \
    void _unicorn_fixture_ ## fixture_name ## _cleanup(UNUSED _unicorn_fixture_type_ ## fixture_name *fixture_name, UNUSED UnicornTestFixture *_unicorn_test_fixture) \
    {


#endif
