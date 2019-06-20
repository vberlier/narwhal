#ifndef TMPDIR_H
#define TMPDIR_H


#include <narwhal/narwhal.h>


DECLARE_FIXTURE(tmpdir, struct { char *path; char *original_path; });


#endif
