#include "unicorn/unicorn.h"


TEST_FIXTURE(sample_collection, UnicornCollection *)
{
    *sample_collection = unicorn_empty_collection();

    CLEANUP_FIXTURE(sample_collection)
    {
        while ((*sample_collection)->count > 0)
        {
            unicorn_collection_pop(*sample_collection);
        }

        unicorn_free_collection(*sample_collection);
    }
}
