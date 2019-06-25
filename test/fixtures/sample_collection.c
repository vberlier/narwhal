#include "narwhal/narwhal.h"

TEST_FIXTURE(sample_collection, NarwhalCollection *)
{
    *sample_collection = narwhal_empty_collection();

    CLEANUP_FIXTURE(sample_collection)
    {
        while ((*sample_collection)->count > 0)
        {
            narwhal_collection_pop(*sample_collection);
        }

        narwhal_free_collection(*sample_collection);
    }
}
