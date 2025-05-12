#pragma once
#ifndef DATA_OBJECT_POOL_H
#define DATA_OBJECT_POOL_H

#include "types.h"

// ----------------------------------------------------------------------------
// ---- Internals -------------------------------------------------------------
// ----------------------------------------------------------------------------

#define _FOR_EACH_OBJECT_POOL_CHUNK(pool, type, iteration_var)                                                                             \
    if (pool->chunk_count > 0)                                                                                                             \
        for (                                                                                                                              \
            struct {                                                                                                                       \
                u32 index;                                                                                                                 \
                type *object;                                                                                                              \
                bool valid;                                                                                                                \
            } iteration_var = {0, (type *)pool->chunks, *(bool *)(pool->chunks + pool->object_size)};                                      \
            iteration_var.index < pool->chunk_count;                                                                                       \
            ++iteration_var.index,                                                                                                         \
              iteration_var.object = (type *)((uptr)iteration_var.object + pool->chunk_size),                                              \
              iteration_var.valid = *(bool *)((uptr)iteration_var.object + pool->object_size))

#define _FOR_EACH_OBJECT_POOL_OBJECT(pool, type, iteration_var)                                                                            \
    if (pool->chunk_count > 0)                                                                                                             \
        for (                                                                                                                              \
            struct {                                                                                                                       \
                u32 index;                                                                                                                 \
                type *object;                                                                                                              \
            } iteration_var = {0, (type *)pool->chunks};                                                                                   \
            iteration_var.index < pool->chunk_count;                                                                                       \
            ++iteration_var.index, iteration_var.object = (type *)((uptr)iteration_var.object + pool->chunk_size))                         \
            if (*(bool *)((uptr)iteration_var.object + pool->object_size))

// ----------------------------------------------------------------------------
// ---- Declarations ----------------------------------------------------------
// ----------------------------------------------------------------------------

#define DATA_OBJECT_POOL_DEFAULT_MEM_SIZE 1024

typedef struct ObjectPool
{
    u32 chunk_count;
    u32 object_count;
    usize object_size;
    byte *chunks;
    usize chunk_size;
    usize mem_size;
} ObjectPool;

/**
 * Creates an data object pool.
 * @param object_size Size of the entities to contain.
 * @return New data object pool.
 */
ObjectPool ObjectPoolCreate(usize object_size);
/**
 * Creates an data object pool with a custom initial memory size.
 * @param object_size Size of the entities to contain.
 * @param initial_mem_size Initial size of the reserved memory.
 * @return New data object pool.
 */
ObjectPool ObjectPoolCreateCustom(usize object_size, usize initial_mem_size);
/**
 * Creates an data object pool of an specific type.
 * @param type Type of the data object to contain.
 * @return New data object pool.
 */
#define ObjectPoolCreateType(type) ObjectPoolCreate(sizeof(type))
/**
 * Creates an data object pool for an data object type with a custom initial memory size.
 * @param type Type of the data object to contain.
 * @param initial_mem_size Initial size of the reserved memory.
 * @return New data object pool.
 */
#define ObjectPoolCreateCustomType(type, initial_stack_size) ObjectPoolCreateCustom(sizeof(type), initial_stack_size)
/**
 * Deletes an data object pool.
 * @param pool Entity pool to delete.
 */
void ObjectPoolDelete(ObjectPool *pool);

/**
 * Adds an data object to an data object pool.
 * @param pool Entity pool to use.
 * @param data object Reference to the memory that contains the data object to include (as a shallow copy) into the data object pool.
 * @return Reference to the new data object inside the memory pool.
 */
void *ObjectPoolAdd(ObjectPool *pool, const void *const data);
/**
 * Removes the data object at the specified index from the data object pool.
 * @param pool Entity pool to use.
 * @param object_index Index of the data object to remove.
 */
void ObjectPoolPop(ObjectPool *pool, u32 object_index);
/**
 * Retrieves the data object at the specified index.
 * @param pool Entity pool to use.
 * @param object_index Index of the data object to retrieve.
 * @return Retrieved data object or NULL if no chunk at the specified index or the data object is invalid.
 */
void *ObjectPoolObjectAt(ObjectPool pool, u32 object_index);
/**
 * Retrieves the data object at the specified index as a given type .
 * @param pool Entity pool to use.
 * @param type Type of the data object.
 * @param object_index Index of the data object to retrieve.
 * @return Retrieved data object casted to type or NULL if no chunk at the specified index or the data object is invalid.
 */
#define ObjectPoolObjectTypeAt(pool, type, object_index) ((type *)ObjectPoolObjectAt(pool, object_index))
/**
 * Retrieves the number of entities inside the data object pool.
 * @param pool Entity pool to check.
 * @return Number of entities inside the data object pool.
 */
u32 ObjectPoolObjectCount(ObjectPool pool);
/**
 * Retrieves the number of chunks that conforms the data object pool.
 * @param pool Entity pool to check.
 * @return Number of chunks that conform the data object pool.
 */
u32 ObjectPoolChunkCount(ObjectPool pool);

/**
 * Custom for-each-loop to iterate over all the chunks of an data object pool.
 * Just define a block of code just like a for-loop to execute for every chunk.
 *
 * @param pool Entity pool to use.
 * @param type Type of the data object.
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index `u32` Index of the current chunk.
 * @param iteration_var.object `type *` Pointer to the object of the chunk.
 * @param iteration_var.valid `bool` Boolean representing if the object inside the chunk is valid.
 *
 * Usage:
 * ```
 * ObjectPool pool = EntityPoolCreateType(float);
 *
 * float f;
 * ObjectPoolAdd(&pool, &(f = 1.0f));
 * ObjectPoolAdd(&pool, &(f = 6.7f));
 *
 * ForEachObjectPoolChunk(pool, float, itr) {
 *     printf("Iteration: %d. Entity: %.2f [%p]. %s\n", itr.index, *itr.data, itr.object, itr.is_used ? "Is alive :)"
 * : "Is dead :(");
 * }
 * ```
 */
#define ForEachObjectPoolChunk(pool, type, iteration_var) _FOR_EACH_OBJECT_POOL_CHUNK((pool), type, iteration_var)

/**
 * Custom for-each-loop to iterate over all the chunks with valid entities of an data object pool.
 * Just define a block of code just like a for-loop to execute for every chunk with valid entities.
 *
 * @param pool Entity pool to use.
 * @param type Type of the data object.
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index `u32` Index of the current chunk.
 * @param iteration_var.object `type *` Pointer to the object of the chunk.
 *
 * Usage:
 * ```
 * ObjectPool pool = EntityPoolCreateType(float);
 *
 * float f;
 * ObjectPoolAdd(&pool, &(f = 1.0f));
 * ObjectPoolAdd(&pool, &(f = 6.7f));
 *
 * ForEachObjectPoolObject(pool, float, itr) {
 *     printf("Iteration: %d. Entity: %.2f [%p]\n", itr.index, *itr.object, itr.object");
 * }
 * ```
 */
#define ForEachObjectPoolObject(pool, type, iteration_var) _FOR_EACH_OBJECT_POOL_OBJECT((pool), type, iteration_var)

#endif // DATA_OBJECT_POOL_H
