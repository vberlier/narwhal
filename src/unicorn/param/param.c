#include <stdlib.h>

#include "unicorn/param/param.h"


/*
 * Test param initialization
 */

static void initialize_test_param(UnicornTestParam *test_param, char *name, void *values, size_t count)
{
    test_param->name = name;
    test_param->index = 0;
    test_param->count = count;
    test_param->values = values;
    test_param->test = NULL;
}

UnicornTestParam *unicorn_new_test_param(char *name, void *values, size_t count)
{
    UnicornTestParam *test_param = malloc(sizeof (UnicornTestParam));
    initialize_test_param(test_param, name, values, count);

    return test_param;
}


/*
 * Cleanup
 */

void unicorn_free_test_param(UnicornTestParam *test_param)
{
    free(test_param);
}
