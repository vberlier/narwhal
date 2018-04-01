#include <stdlib.h>

#include "unicorn/collection/collection.h"


/*
 * Collection initialization
 */

static void initialize_collection(UnicornCollection *collection)
{
    collection->count = 0;
    collection->first = NULL;
    collection->last = NULL;
}

static void collection_item(UnicornCollectionItem *item, void *value)
{
    item->value = value;
    item->previous = NULL;
    item->next = NULL;
}

static UnicornCollectionItem *unicorn_new_collection_item(void *value)
{
    UnicornCollectionItem *item = malloc(sizeof (UnicornCollectionItem));
    collection_item(item, value);

    return item;
}

UnicornCollection *unicorn_empty_collection()
{
    UnicornCollection *collection = malloc(sizeof (UnicornCollection));
    initialize_collection(collection);

    return collection;
}


/*
 * Collection operations
 */

void unicorn_collection_append(UnicornCollection *collection, void *value)
{
    UnicornCollectionItem *new_item = unicorn_new_collection_item(value);
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

void *unicorn_collection_pop(UnicornCollection *collection)
{
    if (collection->count == 0)
    {
        return NULL;
    }

    UnicornCollectionItem *last_item = collection->last;
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

void unicorn_free_collection(UnicornCollection *collection)
{
    free(collection);
}
