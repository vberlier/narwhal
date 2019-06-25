#ifndef NARWHAL_COLLECTION_H
#define NARWHAL_COLLECTION_H

#include <stdbool.h>
#include <stdlib.h>

#include "narwhal/types.h"

struct NarwhalCollection
{
    size_t count;
    NarwhalCollectionItem *first;
    NarwhalCollectionItem *last;
};

struct NarwhalCollectionItem
{
    void *value;
    NarwhalCollectionItem *previous;
    NarwhalCollectionItem *next;
};

NarwhalCollection *narwhal_empty_collection();
void narwhal_collection_append(NarwhalCollection *collection, void *value);
void *narwhal_collection_pop(NarwhalCollection *collection);
void narwhal_free_collection(NarwhalCollection *collection);

// Foreach macros

#define NARWHAL_EACH(item_variable, collection)                                             \
    for (NarwhalCollectionItem *_narwhal_loop_item = (collection)->first;                   \
         _narwhal_loop_item != NULL && ((item_variable) = _narwhal_loop_item->value, true); \
         _narwhal_loop_item = _narwhal_loop_item->next)

#define NARWHAL_REVERSED(item_variable, collection)                                         \
    for (NarwhalCollectionItem *_narwhal_loop_item = (collection)->last;                    \
         _narwhal_loop_item != NULL && ((item_variable) = _narwhal_loop_item->value, true); \
         _narwhal_loop_item = _narwhal_loop_item->previous)

#endif
