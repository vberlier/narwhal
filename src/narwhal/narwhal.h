#ifndef NARWHAL_H
#define NARWHAL_H

#include "narwhal/assertion/assertion.h"
#include "narwhal/collection/collection.h"
#include "narwhal/diff/diff.h"
#include "narwhal/fixture/fixture.h"
#include "narwhal/group/group.h"
#include "narwhal/mock.h"
#include "narwhal/output/output.h"
#include "narwhal/param/param.h"
#include "narwhal/result/result.h"
#include "narwhal/session/session.h"
#include "narwhal/test/test.h"
#include "narwhal/test_utils/test_utils.h"
#include "narwhal/types.h"

int narwhal_run_root_group(NarwhalGroupItemRegistration *root_items, size_t item_count);

#define RUN_TESTS(...)                                                                      \
    ({                                                                                      \
        NarwhalGroupItemRegistration _narwhal_root_items[] = { __VA_ARGS__ };               \
        narwhal_run_root_group(_narwhal_root_items,                                         \
                               sizeof(_narwhal_root_items) / sizeof(*_narwhal_root_items)); \
    })

#endif
