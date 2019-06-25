#include "narwhal/param/param.h"

#include <stdlib.h>
#include <string.h>

#include "narwhal/collection/collection.h"

/*
 * Currently accessible params
 */

NarwhalCollection *_narwhal_current_params = NULL;

/*
 * Test param initialization
 */

static void initialize_test_param(NarwhalTestParam *test_param,
                                  char *name,
                                  void *values,
                                  size_t count,
                                  NarwhalTest *test)
{
    test_param->name = name;
    test_param->index = 0;
    test_param->count = count;
    test_param->values = values;
    test_param->test = test;
}

NarwhalTestParam *narwhal_new_test_param(char *name, void *values, size_t count, NarwhalTest *test)
{
    NarwhalTestParam *test_param = malloc(sizeof(NarwhalTestParam));
    initialize_test_param(test_param, name, values, count, test);

    return test_param;
}

/*
 * Get param from param collection
 */

NarwhalTestParam *narwhal_get_test_param(NarwhalCollection *params, char *param_name)
{
    NarwhalTestParam *test_param;
    NARWHAL_EACH(test_param, params)
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

void narwhal_free_test_param(NarwhalTestParam *test_param)
{
    free(test_param);
}
