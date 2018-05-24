#ifndef UNICORN_TEST_UTILS_H
#define UNICORN_TEST_UTILS_H


#include <stdbool.h>

#include "unicorn/types.h"


extern UnicornOutputCapture _unicorn_default_output_capture;


struct UnicornOutputCapture
{
    bool initialization_phase;
    int stdout_backup;
    int stderr_backup;
    int pipe[2];
};

bool unicorn_capture_output(UnicornOutputCapture *capture, char **output_buffer);


#define CAPTURE_OUTPUT(buffer_name) \
    char *buffer_name = NULL; \
    UnicornOutputCapture _unicorn_capture_ ## buffer_name = _unicorn_default_output_capture; \
    while (unicorn_capture_output(&_unicorn_capture_ ## buffer_name, &buffer_name))


#endif
