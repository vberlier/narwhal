#include "unicorn/unicorn.h"
#include "fixtures/sample_collection.h"


TEST(collection_initialization)
{
    UnicornCollection *collection = unicorn_empty_collection();

    ASSERT_EQ(collection->count, (size_t)0);
    ASSERT_EQ(collection->first, NULL);
    ASSERT_EQ(collection->last, NULL);

    unicorn_free_collection(collection);
}


TEST(collection_operations, sample_collection)
{
    GET_FIXTURE(sample_collection);

    int numbers[] = { 0, 1, 2 };

    unicorn_collection_append(sample_collection, numbers);

    ASSERT_EQ(sample_collection->count, (size_t)1);
    ASSERT_EQ(sample_collection->first, sample_collection->last);

    unicorn_collection_append(sample_collection, numbers + 1);
    unicorn_collection_append(sample_collection, numbers + 2);

    ASSERT_EQ(sample_collection->count, (size_t)3);

    size_t i = 0;

    int *item;
    UNICORN_EACH(item, sample_collection)
    {
        ASSERT_EQ(*item, numbers[i]);
        i++;
    }

    int *last = unicorn_collection_pop(sample_collection);

    ASSERT_EQ(*last, numbers[2]);
    ASSERT_EQ(sample_collection->count, (size_t)2);
}


TEST_GROUP(collection_tests,
{
    collection_initialization,
    collection_operations
})
