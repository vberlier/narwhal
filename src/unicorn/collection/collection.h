#ifndef UNICORN_COLLECTION_H
#define UNICORN_COLLECTION_H


#include <stdlib.h>
#include <stdbool.h>

#include "unicorn/types.h"


struct UnicornCollection
{
    size_t count;
    UnicornCollectionItem *first;
    UnicornCollectionItem *last;
};


struct UnicornCollectionItem
{
    void *value;
    UnicornCollectionItem *previous;
    UnicornCollectionItem *next;
};


UnicornCollection *unicorn_empty_collection();
void unicorn_collection_append(UnicornCollection *collection, void *value);
void *unicorn_collection_pop(UnicornCollection *collection);
void unicorn_free_collection(UnicornCollection *collection);


// Foreach macros

#define UNICORN_EACH(item_variable, collection) \
    for ( \
        UnicornCollectionItem *_unicorn_loop_item = (collection)->first; \
        _unicorn_loop_item != NULL && ((item_variable) = _unicorn_loop_item->value, true); \
        _unicorn_loop_item = _unicorn_loop_item->next \
    )

#define UNICORN_REVERSED(item_variable, collection) \
    for ( \
        UnicornCollectionItem *_unicorn_loop_item = (collection)->last; \
        _unicorn_loop_item != NULL && ((item_variable) = _unicorn_loop_item->value, true); \
        _unicorn_loop_item = _unicorn_loop_item->previous \
    )

#endif
