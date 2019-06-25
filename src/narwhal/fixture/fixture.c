#include "narwhal/fixture/fixture.h"

#include <stdlib.h>
#include <string.h>

#include "narwhal/collection/collection.h"

/*
 * Currently accessible fixtures
 */

NarwhalCollection *_narwhal_current_fixtures = NULL;

/*
 * Test fixture initialization
 */

static void initialize_test_fixture(NarwhalTestFixture *test_fixture,
                                    char *name,
                                    size_t fixture_size,
                                    NarwhalTestFixtureSetup setup,
                                    NarwhalTest *test,
                                    NarwhalTestModifierRegistration *test_modifiers,
                                    size_t modifier_count)
{
    test_fixture->name = name;
    test_fixture->size = fixture_size;
    test_fixture->value = NULL;
    test_fixture->setup = setup;
    test_fixture->cleanup = NULL;
    test_fixture->test = test;
    test_fixture->accessible_fixtures = narwhal_empty_collection();
    test_fixture->accessible_params = narwhal_empty_collection();

    for (size_t i = 0; i < modifier_count; i++)
    {
        NarwhalTestModifierRegistration registration = test_modifiers[i];
        registration(test_fixture->test,
                     test_fixture->accessible_params,
                     test_fixture->accessible_fixtures);
    }
}

NarwhalTestFixture *narwhal_new_test_fixture(char *name,
                                             size_t fixture_size,
                                             NarwhalTestFixtureSetup setup,
                                             NarwhalTest *test,
                                             NarwhalTestModifierRegistration *test_modifiers,
                                             size_t modifier_count)
{
    NarwhalTestFixture *test_fixture = malloc(sizeof(NarwhalTestFixture));
    initialize_test_fixture(
        test_fixture, name, fixture_size, setup, test, test_modifiers, modifier_count);

    return test_fixture;
}

/*
 * Get fixture from fixture collection
 */

NarwhalTestFixture *narwhal_get_test_fixture(NarwhalCollection *fixtures, char *fixture_name)
{
    NarwhalTestFixture *test_fixture;
    NARWHAL_EACH(test_fixture, fixtures)
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

void narwhal_free_test_fixture(NarwhalTestFixture *test_fixture)
{
    while (test_fixture->accessible_fixtures->count > 0)
    {
        narwhal_collection_pop(test_fixture->accessible_fixtures);
    }
    narwhal_free_collection(test_fixture->accessible_fixtures);

    while (test_fixture->accessible_params->count > 0)
    {
        narwhal_collection_pop(test_fixture->accessible_params);
    }
    narwhal_free_collection(test_fixture->accessible_params);

    free(test_fixture);
}
