#ifndef NARWHAL_FIXTURE_TYPES_H
#define NARWHAL_FIXTURE_TYPES_H


typedef struct NarwhalTestFixture NarwhalTestFixture;

typedef void (*NarwhalTestFixtureSetup)(void *value, NarwhalTestFixture *test_fixture);
typedef void (*NarwhalTestFixtureCleanup)(void *value, NarwhalTestFixture *test_fixture);


#endif
