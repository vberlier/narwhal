#include "unicorn/unicorn.h"

#include "test_collection.h"
#include "test_meta_group.h"
#include "test_meta.h"


int main()
{
    return RUN_TESTS(
        collection_tests,
        run_meta_group,
        run_meta_test
    );
}
