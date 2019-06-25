#ifndef NARWHAL_TEST_UTILS_H
#define NARWHAL_TEST_UTILS_H

#include <stdbool.h>

#include "narwhal/types.h"

extern NarwhalOutputCapture _narwhal_default_output_capture;

struct NarwhalOutputCapture
{
    bool initialization_phase;
    int stdout_backup;
    int stderr_backup;
    int pipe[2];
    NarwhalOutputCapture *parent;
};

bool narwhal_capture_output(NarwhalOutputCapture *capture, char **output_buffer);

#define CAPTURE_OUTPUT(buffer_name)                                                        \
    char *buffer_name = NULL;                                                              \
    NarwhalOutputCapture _narwhal_capture_##buffer_name = _narwhal_default_output_capture; \
    while (narwhal_capture_output(&_narwhal_capture_##buffer_name, &buffer_name))

#endif
