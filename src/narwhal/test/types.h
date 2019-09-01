#ifndef NARWHAL_TEST_TYPES_H
#define NARWHAL_TEST_TYPES_H

typedef struct NarwhalTest NarwhalTest;

typedef void (*NarwhalTestModifierRegistration)(NarwhalTest *test,
                                                NarwhalCollection *params,
                                                NarwhalCollection *fixtures);
typedef void (*NarwhalTestFunction)(void);

#endif
