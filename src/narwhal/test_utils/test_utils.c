#include "narwhal/test_utils/test_utils.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "narwhal/test/test.h"
#include "narwhal/utils.h"

NarwhalOutputCapture _narwhal_default_output_capture = { .initialization_phase = true,
                                                         .stdout_backup = -1,
                                                         .stderr_backup = -1,
                                                         .pipe = { -1, -1 },
                                                         .parent = NULL };

/*
 * Initialize capture
 */

static void initialize_output_capture(NarwhalOutputCapture *capture)
{
    if (pipe(capture->pipe) == -1)
    {
        fprintf(stderr, "Failed to create capture pipe.\n");
        exit(EXIT_FAILURE);
    }

    capture->parent = _narwhal_current_test->output_capture;
    _narwhal_current_test->output_capture = capture;

    capture->stdout_backup = dup(STDOUT_FILENO);
    capture->stderr_backup = dup(STDERR_FILENO);

    while (dup2(capture->pipe[1], STDOUT_FILENO) == -1 && errno == EINTR)
        ;
    while (dup2(capture->pipe[1], STDERR_FILENO) == -1 && errno == EINTR)
        ;
}

/*
 * Finalize capture
 */

static void finalize_output_capture(NarwhalOutputCapture *capture, char **output_buffer)
{
    bool terminator_written = write(capture->pipe[1], "", 1) == 1;

    close(capture->pipe[1]);

    dup2(capture->stdout_backup, STDOUT_FILENO);
    dup2(capture->stderr_backup, STDERR_FILENO);

    if (terminator_written)
    {
        FILE *stream = fdopen(capture->pipe[0], "r");

        size_t output_length = (size_t)(narwhal_util_read_stream(stream, output_buffer) - 1);

        if (write(STDOUT_FILENO, *output_buffer, output_length) != (ssize_t)output_length)
        {
            fprintf(stderr, "Failed to write captured output to stdout");
        }

        fclose(stream);
    }
    else
    {
        fprintf(stderr, "Failed to write to capture pipe.\n");
    }

    _narwhal_current_test->output_capture = capture->parent;

    close(capture->pipe[0]);
}

/*
 * Main capture function
 */

bool narwhal_capture_output(NarwhalOutputCapture *capture, char **output_buffer)
{
    fflush(stdout);
    fflush(stderr);

    if (capture->initialization_phase)
    {
        initialize_output_capture(capture);
        capture->initialization_phase = false;

        return true;
    }
    else
    {
        finalize_output_capture(capture, output_buffer);
        auto_free(*output_buffer);

        return false;
    }
}
