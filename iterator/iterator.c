#include <stdlib.h>

typedef struct Iterator Iterator;
typedef struct Iterator {
    void* array;
    size_t size;
    size_t item_size;
    void* current_item;
    size_t current_index;
    void* (*next)(Iterator*);
} Iterator;

#define iterator_array(arr) \
    ((Iterator){            \
        .array = arr, .size = sizeof(array) / sizeof(*array), .item_size = sizeof(*array), .current_item = NULL, .current_index = 0, .next = _next_array})

void* _next_array(Iterator* iterator) {
    if (iterator->current_item == NULL) {
        // Start the iterator
        if (iterator->current_index < iterator->size) {
            iterator->current_index = 0;
            return iterator->current_item = iterator->array;
        }
        // Iterator depleted
        else {
            return NULL;
        }
    }
    // Iterator ongoing
    else if (iterator->current_index < iterator->size - 1) {
        ++iterator->current_index;
        return iterator->current_item = (void*)((size_t)iterator->array + iterator->current_index * iterator->item_size);
    }
    // Iterator finished
    else {
        iterator->current_index = iterator->size;
        return iterator->current_item = NULL;
    }
}
