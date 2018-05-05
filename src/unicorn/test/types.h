#ifndef UNICORN_TEST_TYPES_H
#define UNICORN_TEST_TYPES_H


typedef struct UnicornTest UnicornTest;

typedef void (*UnicornTestModifierRegistration)(UnicornTest *test, UnicornCollection *params, UnicornCollection *fixtures);
typedef void (*UnicornTestFunction)();


#endif
