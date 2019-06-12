#ifndef UNICORN_H
#define UNICORN_H


#include "unicorn/types.h"

#include "unicorn/assertion/assertion.h"
#include "unicorn/collection/collection.h"
#include "unicorn/diff/diff.h"
#include "unicorn/fixture/fixture.h"
#include "unicorn/group/group.h"
#include "unicorn/output/output.h"
#include "unicorn/param/param.h"
#include "unicorn/result/result.h"
#include "unicorn/session/session.h"
#include "unicorn/test/test.h"
#include "unicorn/test_utils/test_utils.h"


int unicorn_run_root_group(UnicornGroupItemRegistration *root_items, size_t item_count);


#define RUN_TESTS(...) \
    ({ \
        UnicornGroupItemRegistration _unicorn_root_items[] = { __VA_ARGS__ }; \
        unicorn_run_root_group(_unicorn_root_items, sizeof (_unicorn_root_items) / sizeof (*_unicorn_root_items)); \
    })


#endif
