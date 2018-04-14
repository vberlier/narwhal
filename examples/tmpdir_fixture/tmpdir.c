#include <stdlib.h>
#include <string.h>
#include <ftw.h>
#include <unistd.h>
#include <unicorn/unicorn.h>


// File tree walk callback that removes every file and directory.

static int remove_item(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)
{
    (void)sb;
    (void)typeflag;
    (void)ftwbuf;

    return remove(fpath);
}


TEST_FIXTURE(tmpdir, struct { char *path; char *original_path; })
{
    // Get the current working directory
    size_t cwd_size = 256;
    tmpdir->original_path = malloc(cwd_size);
    getcwd(tmpdir->original_path, cwd_size);

    // Define the template path for the temporary directory
    char template[] = "/tmp/test_tmpdirXXXXXX";
    tmpdir->path = malloc(sizeof (template));
    strncpy(tmpdir->path, template, sizeof (template));

    // Create the temporary directory and change the working directory
    mkdtemp(tmpdir->path);
    chdir(tmpdir->path);

    CLEANUP_FIXTURE(tmpdir)
    {
        // Change the working directory back to the original directory
        chdir(tmpdir->original_path);

        // Remove everything inside of the temporary directory
        nftw(tmpdir->path, remove_item, 64, FTW_DEPTH | FTW_PHYS);

        // Free allocated memory
        free(tmpdir->path);
        free(tmpdir->original_path);
    }
}
