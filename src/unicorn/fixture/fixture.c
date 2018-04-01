#include <stdlib.h>

#include "unicorn/fixture/fixture.h"


/*
 * Test fixture initialization
 */

static void initialize_test_fixture(UnicornTestFixture *test_fixture, char *name, size_t fixture_size, UnicornTestFixtureSetup setup)
{
    test_fixture->name = name;
    test_fixture->size = fixture_size;
    test_fixture->value = NULL;
    test_fixture->setup = setup;
    test_fixture->cleanup = NULL;
    test_fixture->test = NULL;
}

UnicornTestFixture *unicorn_new_test_fixture(char *name, size_t fixture_size, UnicornTestFixtureSetup setup)
{
    UnicornTestFixture *test_fixture = malloc(sizeof (UnicornTestFixture));
    initialize_test_fixture(test_fixture, name, fixture_size, setup);

    return test_fixture;
}


/*
 * Cleanup
 */

void unicorn_free_test_fixture(UnicornTestFixture *test_fixture)
{
    free(test_fixture);
}
