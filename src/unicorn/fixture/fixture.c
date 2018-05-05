#include <stdlib.h>
#include <string.h>

#include "unicorn/collection/collection.h"
#include "unicorn/fixture/fixture.h"


/*
 * Currently accessible fixtures
 */

UnicornCollection *_unicorn_fixtures = NULL;


/*
 * Test fixture initialization
 */

static void initialize_test_fixture(UnicornTestFixture *test_fixture, char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTest *test, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    test_fixture->name = name;
    test_fixture->size = fixture_size;
    test_fixture->value = NULL;
    test_fixture->setup = setup;
    test_fixture->cleanup = NULL;
    test_fixture->test = test;
    test_fixture->accessible_fixtures = unicorn_empty_collection();
    test_fixture->accessible_params = unicorn_empty_collection();

    for (size_t i = 0; i < modifier_count; i++)
    {
        UnicornTestModifierRegistration registration = test_modifiers[i];
        registration(test_fixture->test, test_fixture->accessible_params, test_fixture->accessible_fixtures);
    }
}

UnicornTestFixture *unicorn_new_test_fixture(char *name, size_t fixture_size, UnicornTestFixtureSetup setup, UnicornTest *test, UnicornTestModifierRegistration *test_modifiers, size_t modifier_count)
{
    UnicornTestFixture *test_fixture = malloc(sizeof (UnicornTestFixture));
    initialize_test_fixture(test_fixture, name, fixture_size, setup, test, test_modifiers, modifier_count);

    return test_fixture;
}


/*
 * Get fixture from fixture collection
 */

UnicornTestFixture *unicorn_get_test_fixture(UnicornCollection *fixtures, char *fixture_name)
{
    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, fixtures)
    {
        if (strcmp(test_fixture->name, fixture_name) == 0)
        {
            return test_fixture;
        }
    }
    return NULL;
}


/*
 * Cleanup
 */

void unicorn_free_test_fixture(UnicornTestFixture *test_fixture)
{
    while (test_fixture->accessible_fixtures->count > 0)
    {
        unicorn_collection_pop(test_fixture->accessible_fixtures);
    }
    unicorn_free_collection(test_fixture->accessible_fixtures);

    while (test_fixture->accessible_params->count > 0)
    {
        unicorn_collection_pop(test_fixture->accessible_params);
    }
    unicorn_free_collection(test_fixture->accessible_params);

    free(test_fixture);
}
