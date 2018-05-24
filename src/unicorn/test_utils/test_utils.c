#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "unicorn/test/test.h"
#include "unicorn/test_utils/test_utils.h"
#include "unicorn/utils.h"


UnicornOutputCapture _unicorn_default_output_capture =
{
    .initialization_phase = true,
    .stdout_backup = -1,
    .stderr_backup = -1,
    .pipe = { -1, -1 }
};


/*
 * Initialize capture
 */

static void initialize_output_capture(UnicornOutputCapture *capture)
{
    capture->stdout_backup = dup(STDOUT_FILENO);
    capture->stderr_backup = dup(STDERR_FILENO);

    pipe(capture->pipe);

    while (dup2(capture->pipe[1], STDOUT_FILENO) == -1 && errno == EINTR);
    while (dup2(capture->pipe[1], STDERR_FILENO) == -1 && errno == EINTR);
}


/*
 * Finalize capture
 */

static void finalize_output_capture(UnicornOutputCapture *capture, char **output_buffer)
{
    write(capture->pipe[1], "", 1);
    close(capture->pipe[1]);

    dup2(capture->stdout_backup, STDOUT_FILENO);
    dup2(capture->stderr_backup, STDERR_FILENO);

    FILE *stream = fdopen(capture->pipe[0], "r");

    unicorn_util_read_stream(stream, output_buffer);

    fclose(stream);
    close(capture->pipe[0]);
}


/*
 * Main capture function
 */

bool unicorn_capture_output(UnicornOutputCapture *capture, char **output_buffer)
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
