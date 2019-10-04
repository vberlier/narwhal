#ifndef TMPDIR_H
#define TMPDIR_H

#include "narwhal.h"

#define TMPDIR_TEMPLATE "/tmp/test_tmpdirXXXXXX"

typedef struct
{
    char path[sizeof(TMPDIR_TEMPLATE)];
    char original_path[512];
} TmpdirFixtureData;

DECLARE_FIXTURE(tmpdir, TmpdirFixtureData);

#endif
