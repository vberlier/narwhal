#ifndef UNICORN_PARAM_H
#define UNICORN_PARAM_H


#include <stdlib.h>

#include "unicorn/types.h"


extern UnicornCollection *_unicorn_params;


struct UnicornTestParam
{
    char *name;
    size_t index;
    size_t count;
    void *values;
    UnicornTest *test;
};

UnicornTestParam *unicorn_new_test_param(char *name, void *values, size_t count, UnicornTest *test);
UnicornTestParam *unicorn_get_test_param(UnicornCollection *params, char *param_name);
void unicorn_free_test_param(UnicornTestParam *test_param);


#define DECLARE_PARAM(param_name, param_type) \
    typedef param_type _unicorn_param_type_ ## param_name; \
    extern _unicorn_param_type_ ## param_name _unicorn_param_ ## param_name[]; \
    void param_name(UnicornTest *test, UnicornCollection *params, UnicornCollection *fixtures)


#define TEST_PARAM(param_name, param_type, ...) \
    DECLARE_PARAM(param_name, param_type); \
    _unicorn_param_type_ ## param_name _unicorn_param_ ## param_name[] = __VA_ARGS__; \
    void param_name(UnicornTest *test, UnicornCollection *params, UNUSED UnicornCollection *fixtures) \
    { \
        unicorn_register_test_param(test, params, #param_name, _unicorn_param_ ## param_name, sizeof (_unicorn_param_ ## param_name) / sizeof (*_unicorn_param_ ## param_name)); \
    }


#define GET_PARAM(param_name) \
    _unicorn_param_type_ ## param_name param_name = \
    ({ \
        UnicornTestParam *_unicorn_test_param_ ## param_name = unicorn_get_test_param(_unicorn_params, #param_name); \
        if (_unicorn_test_param_ ## param_name == NULL) FAIL("Parameter \"%s\" hasn't been applied to the current context.", #param_name); \
        ((_unicorn_param_type_ ## param_name *)_unicorn_test_param_ ## param_name->values)[_unicorn_test_param_ ## param_name->index]; \
    })


#endif
