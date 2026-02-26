#pragma once
#ifndef LIST_H_
#define LIST_H_
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>

#pragma region internals

typedef struct ListNode ListNode;
struct ListNode {
    ListNode* next;
    alignas(8) char data[];
};

typedef struct {
    ListNode* head;
    ListNode* back;
    ListNode* free;
    size_t size;
} ListDefinition;

ListNode* _list_alloc_node(ListDefinition* def, size_t data_size) {
    ListNode* node = def->free;
    if (node) {
        def->free = def->free->next;
    } else {
        node = malloc(sizeof(ListNode*) + data_size);
    }
    assert(node);
    return node;
}

void* _list_alloc_front(ListDefinition* def, size_t data_size) {
    assert(def);
    ListNode* node = _list_alloc_node(def, data_size);  // Alloc new node
    node->next = def->head;                             // Append head node to new node
    def->head = node;                                   // Update head ref
    if (def->size == 0) { def->back = node; }           // Update back ref if only node in list
    ++def->size;                                        // Update size
    return node->data;
}

void* _list_alloc_back(ListDefinition* def, size_t data_size) {
    assert(def);
    ListNode* node = _list_alloc_node(def, data_size);  // Alloc new node
    node->next = NULL;                                  // Set next node to NULL
    if (def->size > 0) {
        def->back->next = node;  // Append new node at the last position
        def->back = node;        // Update back ref
    } else {
        def->back = def->head = node;  // Update back and head ref if only node in list
    }
    ++def->size;  // Update size
    return node->data;
}

void* _list_alloc_at(ListDefinition* def, size_t data_size, size_t index) {
    assert(def);
    assert(index <= def->size);
    if (index == 0) {
        return _list_alloc_front(def, data_size);  // If first node, alloc at front
    } else if (index == def->size) {
        return _list_alloc_back(def, data_size);  // If last node, alloc at back
    } else {
        ListNode* node = _list_alloc_node(def, data_size);  // Alloc new node
        ListNode** aux = &def->head;
        while (--index) { aux = &(*aux)->next; }  // Find node where to append
        node->next = (*aux)->next;                // Ref next node into new node
        (*aux)->next = node;                      // Append node at position
        ++def->size;                              // Update size
        return node->data;
    }
}

void _list_remove_at(ListDefinition* def, size_t index) {
    assert(def);
    assert(index < def->size);
    ListNode* free;
    if (index == 0) {
        free = def->head;                                // Save removed node
        def->head = def->head->next;                     // Remove node from list
        if (--def->size == index) { def->back = NULL; }  // If list empty, remove back ref
    } else {
        ListNode* node = def->head;
        size_t counter = index;
        while (--counter) { node = node->next; }         // Find the previous node
        free = node->next;                               // Save removed node
        node->next = node->next->next;                   // Remove node from list
        if (--def->size == index) { def->back = node; }  // If removed last node, update back ref
    }
    free->next = def->free;  // Append free list to new free node
    def->free = free;        // Save node to free list
}

void* _list_item_at(ListDefinition* def, size_t index) {
    assert(def);
    if (index == 0) {
        return &def->head->data;
    } else if (index == def->size - 1) {
        return &def->back->data;
    } else {
        ListNode* node = def->head;
        while (index--) { node = node->next; }
        return &node->data;
    }
}

void _list_clear(ListDefinition* def) {
    assert(def);
    if (def->size > 0) {
        def->back->next = def->free;
        def->free = def->head;
        def->head = def->back = NULL;
        def->size = 0;
    }
}

void _list_delete(ListDefinition* def) {
    assert(def);

    ListNode *aux, *node = def->free;
    while (node != NULL) {
        aux = node;
        node = node->next;
        free(aux);
    }

    node = def->head;
    while (node != NULL) {
        ListNode* aux = node;
        node = node->next;
        free(aux);
    }
}

void* _list_data_next(void* data) {
    assert(data);
    ListNode* node = (ListNode*)(void*)((char*)data - offsetof(ListNode, data));  // Calculates the ref to the node
    node = node->next;                                                            // Reference to next node
    return node ? node->data : NULL;
}

#define _list_data_first(list) (typeof((list)->payload))((list)->def.head ? (list)->def.head->data : NULL)

#pragma endregion

/**
 * Single linked list generic type
 * @param type Data type of the list
 */
#define List(type)          \
    union {                 \
        ListDefinition def; \
        type* payload;      \
    }

/**
 * Allocates a new node at the front of a list
 * @param list List where to allocate the node
 * @returns Reference to the allocated node
 */
#define list_alloc_front(list) ((typeof((list)->payload))_list_alloc_front(&((list)->def), sizeof(*(list)->payload)))
/**
 * Allocates a new node at the back of a list
 * @param list List where to allocate the node
 * @returns Reference to the allocated node
 */
#define list_alloc_back(list) ((typeof((list)->payload))_list_alloc_back(&((list)->def), sizeof(*(list)->payload)))
/**
 * Allocates a new node in a list
 * @param list List where to allocate the node
 * @returns Reference to the allocated node
 */
#define list_alloc_at(list, index) ((typeof((list)->payload))_list_alloc_at(&((list)->def), sizeof(*(list)->payload), index))

/**
 * Adds a node to the front of a list
 * @param list List to append the node to
 * @param item Item to append to the list
 */
#define list_prepend(list, item) \
    do { *list_alloc_front(list) = item; } while (0)
/**
 * Adds a node to the back of a list
 * @param list List to append the node to
 * @param item Item to append to the list
 */
#define list_push(list, item) \
    do { *list_alloc_back(list) = item; } while (0)
/**
 * Adds a node to a list
 * @param list List to append the node to
 * @param item Item to append to the list
 * @param index Positon to insert the node into
 */
#define list_add(list, item, index) \
    do { *list_alloc_at(list, index) = item; } while (0)

/**
 * Removes the first node from a list
 * @param list List to remove the node from
 */
#define list_behead(list) \
    do { _list_remove_at(&(list)->def, 0); } while (0)
/**
 * Removes the last node from a list
 * @param list List to remove the node from
 */
#define list_pop(list) \
    do { _list_remove_at(&(list)->def, (list)->def.size - 1); } while (0)
/**
 * Removes a node from a list
 * @param list List to remove the node from
 */
#define list_remove(list, index) \
    do { _list_remove_at(&(list)->def, index); } while (0)

/**
 * Retrieves the first element of a list
 * @param list List to retrieve the element from
 * @returns Reference to the item
 */
#define list_item_first(list) ((typeof((list)->payload))_list_item_at(&(list)->def, 0))
/**
 * Retrieves the last element of a list
 * @param list List to retrieve the element from
 * @returns Reference to the item
 */
#define list_item_last(list) ((typeof((list)->payload))_list_item_at(&(list)->def, (list)->def.size - 1))
/**
 * Retrieves an element from a list
 * @param list List to retrieve the element from
 * @returns Reference to the item
 */
#define list_item_at(list, index) ((typeof((list)->payload))_list_item_at(&(list)->def, index))

/**
 * Retrieves the size of a list
 * @param list List to measure
 */
#define list_size(list) ((list).size)

/**
 * Clears the contents of a list
 * @param list List to clear
 */
#define list_clear(list) \
    do { _list_clear(&(list)->def); } while (0)

/**
 * Deletes a list and frees its resources
 * @param list List to delete
 */
#define list_delete(list) \
    do { _list_delete(&(list)->def); } while (0)

/**
 * Iterates all the nodes of a list in order
 * @param iter Iterator parameter which will contain the value of the node corresponding to the current iteration
 * @param list List to iterate
 */
#define list_for(iter, list) for (typeof((list)->payload) iter = _list_data_first(list); iter != NULL; iter = _list_data_next(iter))

#endif  // LIST_H_