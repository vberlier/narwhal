#ifndef NARWHAL_PARAM_H
#define NARWHAL_PARAM_H


#include <stdlib.h>

#include "narwhal/types.h"


extern NarwhalCollection *_narwhal_current_params;


struct NarwhalTestParam
{
    char *name;
    size_t index;
    size_t count;
    void *values;
    NarwhalTest *test;
};

NarwhalTestParam *narwhal_new_test_param(char *name, void *values, size_t count, NarwhalTest *test);
NarwhalTestParam *narwhal_get_test_param(NarwhalCollection *params, char *param_name);
void narwhal_free_test_param(NarwhalTestParam *test_param);


#define DECLARE_PARAM(param_name, param_type) \
    typedef param_type _narwhal_param_type_ ## param_name; \
    extern _narwhal_param_type_ ## param_name _narwhal_param_ ## param_name[]; \
    void param_name(NarwhalTest *test, NarwhalCollection *params, NarwhalCollection *fixtures)


#define TEST_PARAM(param_name, param_type, ...) \
    DECLARE_PARAM(param_name, param_type); \
    _narwhal_param_type_ ## param_name _narwhal_param_ ## param_name[] = __VA_ARGS__; \
    void param_name(NarwhalTest *test, NarwhalCollection *params, UNUSED NarwhalCollection *fixtures) \
    { \
        narwhal_register_test_param(test, params, #param_name, _narwhal_param_ ## param_name, sizeof (_narwhal_param_ ## param_name) / sizeof (*_narwhal_param_ ## param_name)); \
    }


#define GET_PARAM(param_name) \
    _narwhal_param_type_ ## param_name param_name = \
    ({ \
        NarwhalTestParam *_narwhal_test_param_ ## param_name = narwhal_get_test_param(_narwhal_current_params, #param_name); \
        if (_narwhal_test_param_ ## param_name == NULL) FAIL("Parameter \"%s\" hasn't been applied to the current context.", #param_name); \
        ((_narwhal_param_type_ ## param_name *)_narwhal_test_param_ ## param_name->values)[_narwhal_test_param_ ## param_name->index]; \
    })


#endif