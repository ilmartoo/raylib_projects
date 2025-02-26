#pragma once
#ifndef __ILMARTO_DATA_OBJECT_POOL_H_
#define __ILMARTO_DATA_OBJECT_POOL_H_

#include "types.h"

// -- Internals -----------------------------------
#define __ILMARTO_DATA_OBJECT_POOL_FOR_EACH_CHUNK(pool, type, iteration_var)                                                               \
    if (pool->chunk_count > 0)                                                                                                             \
        for (                                                                                                                              \
            struct {                                                                                                                       \
                u32 index;                                                                                                                 \
                type *data_object;                                                                                                         \
                bool valid;                                                                                                                \
            } iteration_var = {0, (type *)pool->chunks, *(bool *)(pool->chunks + pool->data_object_size)};                                 \
            iteration_var.index < pool->chunk_count;                                                                                       \
            ++iteration_var.index, iteration_var.data_object = (type *)((uptr)iteration_var.data_object + pool->chunk_size),               \
              iteration_var.valid = *(bool *)((uptr)iteration_var.data_object + pool->data_object_size))

#define __ILMARTO_DATA_OBJECT_POOL_FOR_EACH_OBJECT(pool, type, iteration_var)                                                              \
    if (pool->chunk_count > 0)                                                                                                             \
        for (                                                                                                                              \
            struct {                                                                                                                       \
                u32 index;                                                                                                                 \
                type *data_object;                                                                                                         \
            } iteration_var = {0, (type *)pool->chunks};                                                                                   \
            iteration_var.index < pool->chunk_count;                                                                                       \
            ++iteration_var.index, iteration_var.data_object = (type *)((uptr)iteration_var.data_object + pool->chunk_size))               \
            if (*(bool *)((uptr)iteration_var.data_object + pool->data_object_size))
// ------------------------------------------------

#define __ILMARTO_DATA_OBJECT_POOL_DEFAULT_MEM_SIZE 1024

typedef struct DataObjectPool
{
    u32 chunk_count;
    u32 data_object_count;
    usize data_object_size;
    byte *chunks;
    usize chunk_size;
    usize mem_size;
} DataObjectPool;

/**
 * Creates an data object pool.
 * @param data_object_size Size of the entities to contain.
 * @return New data object pool.
 */
DataObjectPool data_object_pool_create(usize data_object_size);
/**
 * Creates an data object pool with a custom initial memory size.
 * @param data_object_size Size of the entities to contain.
 * @param initial_mem_size Initial size of the reserved memory.
 * @return New data object pool.
 */
DataObjectPool data_object_pool_create_custom(usize data_object_size, usize initial_mem_size);
/**
 * Creates an data object pool of an specific type.
 * @param type Type of the data object to contain.
 * @return New data object pool.
 */
#define data_object_pool_create_type(type) data_object_pool_create(sizeof(type))
/**
 * Creates an data object pool for an data object type with a custom initial memory size.
 * @param type Type of the data object to contain.
 * @param initial_mem_size Initial size of the reserved memory.
 * @return New data object pool.
 */
#define data_object_pool_create_custom_type(type, initial_stack_size) data_object_pool_create_custom(sizeof(type), initial_stack_size)
/**
 * Deletes an data object pool.
 * @param pool Entity pool to delete.
 */
void data_object_pool_release(DataObjectPool *pool);

/**
 * Adds an data object to an data object pool.
 * @param pool Entity pool to use.
 * @param data object Reference to the memory that contains the data object to include (as a shallow copy) into the data object pool.
 * @return Reference to the new data object inside the memory pool.
 */
void *data_object_pool_add(DataObjectPool *pool, const void *data_object);
/**
 * Removes the data object at the specified index from the data object pool.
 * @param pool Entity pool to use.
 * @param data_object_index Index of the data object to remove.
 */
void data_object_pool_pop(DataObjectPool *pool, u32 data_object_index);
/**
 * Retrieves the data object at the specified index.
 * @param pool Entity pool to use.
 * @param data_object_index Index of the data object to retrieve.
 * @return Retrieved data object or NULL if no chunk at the specified index or the data object is invalid.
 */
void *data_object_pool_data_object_at(DataObjectPool pool, u32 data_object_index);
/**
 * Retrieves the data object at the specified index as a given type .
 * @param pool Entity pool to use.
 * @param type Type of the data object.
 * @param data_object_index Index of the data object to retrieve.
 * @return Retrieved data object casted to type or NULL if no chunk at the specified index or the data object is invalid.
 */
#define data_object_pool_data_object_type_at(pool, type, data_object_index)                                                                \
    ((type *)data_object_pool_data_object_at(pool, data_object_index))
/**
 * Retrieves the number of entities inside the data object pool.
 * @param pool Entity pool to check.
 * @return Number of entities inside the data object pool.
 */
u32 data_object_pool_object_count(DataObjectPool pool);
/**
 * Retrieves the number of chunks that conforms the data object pool.
 * @param pool Entity pool to check.
 * @return Number of chunks that conform the data object pool.
 */
u32 data_object_pool_chunk_count(DataObjectPool pool);

/**
 * Custom for-each-loop to iterate over all the chunks of an data object pool.
 * Just define a block of code just like a for-loop to execute for every chunk.
 *
 * @param pool Entity pool to use.
 * @param type Type of the data object.
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index Index of the current chunk.
 * @param iteration_var.data_object Pointer to the data object of the chunk.
 * @param iteration_var.valid Boolean representing if the data object inside the chunk is valid.
 *
 * Usage:
 * ```
 * DataObjectPool pool = EntityPoolCreateType(float);
 *
 * float f;
 * data_object_pool_add(&pool, &(f = 1.0f));
 * data_object_pool_add(&pool, &(f = 6.7f));
 *
 * data_object_pool_for_each_chunk(pool, float, itr) {
 *     printf("Iteration: %d. Entity: %.2f [%p]. %s\n", itr.index, *itr.data_object, itr.data_object, itr.is_used ? "Is alive :)"
 * : "Is dead :(");
 * }
 * ```
 */
#define data_object_pool_for_each_chunk(pool, type, iteration_var) __ILMARTO_DATA_OBJECT_POOL_FOR_EACH_CHUNK((pool), type, iteration_var)

/**
 * Custom for-each-loop to iterate over all the chunks with valid entities of an data object pool.
 * Just define a block of code just like a for-loop to execute for every chunk with valid entities.
 *
 * @param pool Entity pool to use.
 * @param type Type of the data object.
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index Index of the current chunk.
 * @param iteration_var.data_object Pointer to the data object of the chunk.
 *
 * Usage:
 * ```
 * DataObjectPool pool = EntityPoolCreateType(float);
 *
 * float f;
 * data_object_pool_add(&pool, &(f = 1.0f));
 * data_object_pool_add(&pool, &(f = 6.7f));
 *
 * data_object_pool_for_each_data_object(pool, float, itr) {
 *     printf("Iteration: %d. Entity: %.2f [%p]\n", itr.index, *itr.data_object, itr.data_object");
 * }
 * ```
 */
#define data_object_pool_for_each_object(pool, type, iteration_var) __ILMARTO_DATA_OBJECT_POOL_FOR_EACH_OBJECT((pool), type, iteration_var)

#endif // __ILMARTO_DATA_OBJECT_POOL_H_
