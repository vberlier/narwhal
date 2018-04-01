#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#include "unicorn/collection/collection.h"
#include "unicorn/fixture/fixture.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"
#include "unicorn/test/test.h"


/*
 * Test creation
 */

static void initialize_test(UnicornTest *test, char *name, char *filename, size_t line_number, UnicornTestFunction function,  UnicornTestItemRegistration *test_items, size_t item_count)
{
    test->name = name;
    test->filename = filename;
    test->line_number = line_number;
    test->group = NULL;
    test->function = function;
    test->resources = unicorn_empty_collection();
    test->fixtures = unicorn_empty_collection();
    test->params = unicorn_empty_collection();
    test->result = NULL;

    for (size_t i = 0; i < item_count; i++)
    {
        UnicornTestItemRegistration registration = test_items[i];
        registration(test);
    }
}

UnicornTest *unicorn_new_test(char *name, char *filename, size_t line_number, UnicornTestFunction function, UnicornTestItemRegistration *test_items, size_t item_count)
{
    UnicornTest *test = malloc(sizeof (UnicornTest));
    initialize_test(test, name, filename, line_number, function, test_items, item_count);

    return test;
}


/*
 * Test resources
 */

void unicorn_free_after_test(UnicornTest *test, void *resource)
{
    unicorn_collection_append(test->resources, resource);
}

void *unicorn_test_resource(UnicornTest *test, size_t size)
{
    void *resource = malloc(size);
    unicorn_free_after_test(test, resource);
    return resource;
}

void unicorn_free_test_resources(UnicornTest *test)
{
    while (test->resources->count > 0)
    {
        void *test_resource = unicorn_collection_pop(test->resources);
        free(test_resource);
    }
}


/*
 * Run test
 */

static void test_start(UnicornTest *test)
{
    test->result = unicorn_new_test_result();
    test->result->test = test;

    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        test_fixture->cleanup = NULL;
        test_fixture->value = unicorn_test_resource(test, test_fixture->size);
        test_fixture->setup(test_fixture->value, test_fixture);
    }

    UnicornTestParam *test_param;
    UNICORN_EACH(test_param, test->params)
    {
        UnicornTestParamSnapshot *param_snapshot = unicorn_new_test_param_snapshot(test_param);
        unicorn_collection_append(test->result->param_snapshots, param_snapshot);
    }

    test->result->start_time = clock();
}

static void test_end(UnicornTest *test)
{
    test->result->end_time = clock();

    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        if (test_fixture->cleanup != NULL)
        {
            test_fixture->cleanup(test_fixture->value, test_fixture);
        }
    }

    unicorn_free_test_resources(test);
}

void unicorn_run_test(UnicornTest *test)
{
    test_start(test);
    test->function(test, test);
    test_end(test);
}


/*
 * Test fixtures
 */

void unicorn_register_test_fixture(UnicornTest *test, char *name, size_t fixture_size, UnicornTestFixtureSetup setup)
{
    UnicornTestFixture *test_fixture = unicorn_new_test_fixture(name, fixture_size, setup);
    test_fixture->test = test;

    unicorn_collection_append(test->fixtures, test_fixture);
}

UnicornTestFixture *unicorn_get_test_fixture(UnicornTest *test, char *fixture_name)
{
    UnicornTestFixture *test_fixture;
    UNICORN_EACH(test_fixture, test->fixtures)
    {
        if (strcmp(test_fixture->name, fixture_name) == 0)
        {
            return test_fixture;
        }
    }
    return NULL;
}


/*
 * Test params
 */

void unicorn_register_test_param(UnicornTest *test, char *name, void *values, size_t count)
{
    UnicornTestParam *test_param = unicorn_new_test_param(name, values, count);
    test_param->test = test;

    unicorn_collection_append(test->params, test_param);
}

UnicornTestParam *unicorn_get_test_param(UnicornTest *test, char *param_name)
{
    UnicornTestParam *test_param;
    UNICORN_EACH(test_param, test->params)
    {
        if (strcmp(test_param->name, param_name) == 0)
        {
            return test_param;
        }
    }
    return NULL;
}


/*
 * Cleanup
 */

void unicorn_free_test(UnicornTest *test)
{
    while (test->fixtures->count > 0)
    {
        UnicornTestFixture *test_fixture = unicorn_collection_pop(test->fixtures);
        unicorn_free_test_fixture(test_fixture);
    }
    unicorn_free_collection(test->fixtures);

    while (test->params->count > 0)
    {
        UnicornTestParam *test_param = unicorn_collection_pop(test->params);
        unicorn_free_test_param(test_param);
    }
    unicorn_free_collection(test->params);

    unicorn_free_collection(test->resources);

    free(test);
}
