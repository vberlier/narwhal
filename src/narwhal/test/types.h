#ifndef NARWHAL_TEST_TYPES_H
#define NARWHAL_TEST_TYPES_H

#include "narwhal/collection/types.h"

typedef struct NarwhalTest NarwhalTest;

typedef void (*NarwhalTestModifierRegistrationFunction)(NarwhalTest *test,
                                                        NarwhalCollection *params,
                                                        NarwhalCollection *fixtures,
                                                        void *args);
typedef struct NarwhalTestModifierRegistration NarwhalTestModifierRegistration;

typedef void (*NarwhalTestFunction)(void);
typedef void (*NarwhalResetAllMocksFunction)(void);

typedef struct NarwhalTimeoutModifierArgs NarwhalTimeoutModifierArgs;

#endif
