#ifndef UNICORN_PARAM_H
#define UNICORN_PARAM_H


#include <stdlib.h>

#include "unicorn/types.h"


struct UnicornTestParam
{
    char *name;
    size_t index;
    size_t count;
    void *values;
    UnicornTest *test;
};

UnicornTestParam *unicorn_new_test_param(char *name, void *values, size_t count);
void unicorn_free_test_param(UnicornTestParam *test_param);


#define DECLARE_PARAM(param_name, param_type, ...) \
    typedef param_type _unicorn_param_type_ ## param_name; \
    extern _unicorn_param_type_ ## param_name _unicorn_param_ ## param_name[]; \
    void param_name(UnicornTest *test)


#define TEST_PARAM(param_name, param_type, ...) \
    DECLARE_PARAM(param_name, param_type); \
    _unicorn_param_type_ ## param_name _unicorn_param_ ## param_name[] = __VA_ARGS__; \
    void param_name(UnicornTest *test) \
    { \
        unicorn_register_test_param(test, #param_name, _unicorn_param_ ## param_name, sizeof (_unicorn_param_ ## param_name) / sizeof (*_unicorn_param_ ## param_name)); \
    }


#define GET_PARAM(param_name) \
    _unicorn_param_type_ ## param_name param_name = \
    ({ \
        UnicornTestParam *_unicorn_test_param_ ## param_name = unicorn_get_test_param(test, #param_name); \
        ((_unicorn_param_type_ ## param_name *)_unicorn_test_param_ ## param_name->values)[_unicorn_test_param_ ## param_name->index]; \
    })


#endif
