#include "unicorn/unicorn.h"

#include "test_capture_output.h"
#include "test_collection.h"
#include "test_diff.h"
#include "test_meta_group.h"
#include "test_meta.h"


int main()
{
    return RUN_TESTS(
        capture_output_tests,
        collection_tests,
        diff_tests,
        run_meta_group,
        run_meta_test
    );
}
