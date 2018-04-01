#ifndef UNICORN_FIXTURE_TYPES_H
#define UNICORN_FIXTURE_TYPES_H


typedef struct UnicornTestFixture UnicornTestFixture;

typedef void (*UnicornTestFixtureSetup)(void *value, UnicornTestFixture *test_fixture);
typedef void (*UnicornTestFixtureCleanup)(void *value, UnicornTestFixture *test_fixture);


#endif
