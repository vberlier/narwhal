#ifndef NARWHAL_H
#define NARWHAL_H

#include "narwhal/assertion/assertion.h"
#include "narwhal/collection/collection.h"
#include "narwhal/diff/diff.h"
#include "narwhal/discovery/discovery.h"
#include "narwhal/fixture/fixture.h"
#include "narwhal/group/group.h"
#include "narwhal/hexdump/hexdump.h"
#include "narwhal/output/output.h"
#include "narwhal/param/param.h"
#include "narwhal/result/result.h"
#include "narwhal/session/session.h"
#include "narwhal/test/test.h"
#include "narwhal/test_utils/test_utils.h"
#include "narwhal/types.h"

int narwhal_run_tests(NarwhalGroupItemRegistration *tests, size_t test_count);
int narwhal_run_root_group(NarwhalTestGroup *root_group);

#define RUN_TESTS(...)                                                          \
    narwhal_run_tests((NarwhalGroupItemRegistration[]){ __VA_ARGS__ },          \
                      sizeof((NarwhalGroupItemRegistration[]){ __VA_ARGS__ }) / \
                          sizeof(NarwhalGroupItemRegistration))

#endif
