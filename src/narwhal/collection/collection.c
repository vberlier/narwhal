#include "narwhal/collection/collection.h"

#include <stdlib.h>

/*
 * Collection initialization
 */

static void initialize_collection(NarwhalCollection *collection)
{
    collection->count = 0;
    collection->first = NULL;
    collection->last = NULL;
}

static void collection_item(NarwhalCollectionItem *item, void *value)
{
    item->value = value;
    item->previous = NULL;
    item->next = NULL;
}

static NarwhalCollectionItem *narwhal_new_collection_item(void *value)
{
    NarwhalCollectionItem *item = malloc(sizeof(NarwhalCollectionItem));
    collection_item(item, value);

    return item;
}

NarwhalCollection *narwhal_empty_collection()
{
    NarwhalCollection *collection = malloc(sizeof(NarwhalCollection));
    initialize_collection(collection);

    return collection;
}

/*
 * Collection operations
 */

void narwhal_collection_append(NarwhalCollection *collection, void *value)
{
    NarwhalCollectionItem *new_item = narwhal_new_collection_item(value);
    new_item->previous = collection->last;
    collection->last = new_item;
    collection->count++;

    if (collection->count > 1)
    {
        new_item->previous->next = new_item;
    }
    else
    {
        collection->first = new_item;
    }
}

void *narwhal_collection_pop(NarwhalCollection *collection)
{
    if (collection->count == 0)
    {
        return NULL;
    }

    NarwhalCollectionItem *last_item = collection->last;
    collection->last = last_item->previous;
    collection->count--;

    void *value = last_item->value;
    free(last_item);

    if (collection->count > 0)
    {
        collection->last->next = NULL;
    }
    else
    {
        collection->first = NULL;
    }

    return value;
}

/*
 * Cleanup
 */

void narwhal_free_collection(NarwhalCollection *collection)
{
    free(collection);
}
