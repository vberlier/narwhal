#ifndef UNICORN_TEST_TYPES_H
#define UNICORN_TEST_TYPES_H


typedef struct UnicornTest UnicornTest;

typedef void (*UnicornTestItemRegistration)(UnicornTest *test);
typedef void (*UnicornTestFunction)(UnicornTest *test, UnicornTest *_unicorn_test);


#endif
