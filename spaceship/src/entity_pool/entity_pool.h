#pragma once
#ifndef __ILMARTO_ENTITY_POOL_H_
#define __ILMARTO_ENTITY_POOL_H_

#include "types.h"

// -- Internals -----------------------------------
#define __ILMARTO_ENTITY_POOL_FOR_EACH_CHUNK(pool, type, iteration_var)                                                                    \
    if (pool->chunk_count > 0)                                                                                                             \
        for (                                                                                                                              \
            struct {                                                                                                                       \
                u32 index;                                                                                                                 \
                type *entity;                                                                                                              \
                bool valid;                                                                                                                \
            } iteration_var = {0, (type *)pool->chunks, *(bool *)(pool->chunks + pool->entity_size)};                                      \
            iteration_var.index < pool->chunk_count;                                                                                       \
            ++iteration_var.index, iteration_var.entity = (type *)((uptr)iteration_var.entity + pool->chunk_size),                         \
              iteration_var.valid = *(bool *)((uptr)iteration_var.entity + pool->entity_size))

#define __ILMARTO_ENTITY_POOL_FOR_EACH_ENTITY(pool, type, iteration_var)                                                                   \
    if (pool->chunk_count > 0)                                                                                                             \
        for (                                                                                                                              \
            struct {                                                                                                                       \
                u32 index;                                                                                                                 \
                type *entity;                                                                                                              \
            } iteration_var = {0, (type *)pool->chunks};                                                                                   \
            iteration_var.index < pool->chunk_count;                                                                                       \
            ++iteration_var.index, iteration_var.entity = (type *)((uptr)iteration_var.entity + pool->chunk_size))                         \
            if (*(bool *)((uptr)iteration_var.entity + pool->entity_size))
// ------------------------------------------------

#define __ILMARTO_ENTITY_POOL_DEFAULT_MEM_SIZE 1024

typedef struct EntityPool
{
    u32 chunk_count;
    u32 entity_count;
    usize entity_size;
    byte *chunks;
    usize chunk_size;
    usize mem_size;
} EntityPool;

/**
 * Creates an entity pool.
 * @param entity_size Size of the entities to contain.
 * @return New entity pool.
 */
EntityPool entity_pool_create(usize entity_size);
/**
 * Creates an entity pool with a custom initial memory size.
 * @param entity_size Size of the entities to contain.
 * @param initial_mem_size Initial size of the reserved memory.
 * @return New entity pool.
 */
EntityPool entity_pool_create_custom(usize entity_size, usize initial_mem_size);
/**
 * Creates an entity pool of an specific type.
 * @param type Type of the entity to contain.
 * @return New entity pool.
 */
#define entity_pool_create_type(type) entity_pool_create(sizeof(type))
/**
 * Creates an entity pool for an entity type with a custom initial memory size.
 * @param type Type of the entity to contain.
 * @param initial_mem_size Initial size of the reserved memory.
 * @return New entity pool.
 */
#define entity_pool_create_custom_type(type, initial_stack_size) entity_pool_create_custom(sizeof(type), initial_stack_size)
/**
 * Deletes an entity pool.
 * @param pool Entity pool to delete.
 */
void entity_pool_release(EntityPool *pool);

/**
 * Adds an entity to an entity pool.
 * @param pool Entity pool to use.
 * @param entity Reference to the memory that contains the entity to include (as a shallow copy) into the entity pool.
 * @return Reference to the new entity inside the memory pool.
 */
void *entity_pool_add(EntityPool *pool, const void *entity);
/**
 * Removes the entity at the specified index from the entity pool.
 * @param pool Entity pool to use.
 * @param entity_index Index of the entity to remove.
 */
void entity_pool_pop(EntityPool *pool, u32 entity_index);
/**
 * Retrieves the entity at the specified index.
 * @param pool Entity pool to use.
 * @param entity_index Index of the entity to retrieve.
 * @return Retrieved entity or NULL if no chunk at the specified index or the entity is invalid.
 */
void *entity_pool_entity_at(EntityPool pool, u32 entity_index);
/**
 * Retrieves the entity at the specified index as a given type .
 * @param pool Entity pool to use.
 * @param type Type of the entity.
 * @param entity_index Index of the entity to retrieve.
 * @return Retrieved entity casted to type or NULL if no chunk at the specified index or the entity is invalid.
 */
#define entity_pool_entity_type_at(pool, type, entity_index) ((type *)entity_pool_entity_at(pool, entity_index))
/**
 * Retrieves the number of entities inside the entity pool.
 * @param pool Entity pool to check.
 * @return Number of entities inside the entity pool.
 */
u32 entity_pool_entity_count(EntityPool pool);
/**
 * Retrieves the number of chunks that conforms the entity pool.
 * @param pool Entity pool to check.
 * @return Number of chunks that conform the entity pool.
 */
u32 entity_pool_chunk_count(EntityPool pool);

/**
 * Custom for-each-loop to iterate over all the chunks of an entity pool.
 * Just define a block of code just like a for-loop to execute for every chunk.
 *
 * @param pool Entity pool to use.
 * @param type Type of the entity.
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index Index of the current chunk.
 * @param iteration_var.entity Pointer to the entity of the chunk.
 * @param iteration_var.valid Boolean representing if the entity inside the chunk is valid.
 *
 * Usage:
 * ```
 * EntityPool pool = EntityPoolCreateType(float);
 *
 * float f;
 * entity_pool_add(&pool, &(f = 1.0f));
 * entity_pool_add(&pool, &(f = 6.7f));
 *
 * entity_pool_for_each_chunk(pool, float, itr) {
 *     printf("Iteration: %d. Entity: %.2f [%p]. %s\n", itr.index, *itr.entity, itr.entity, itr.is_used ? "Is alive :)"
 * : "Is dead :(");
 * }
 * ```
 */
#define entity_pool_for_each_chunk(pool, type, iteration_var) __ILMARTO_ENTITY_POOL_FOR_EACH_CHUNK((pool), type, iteration_var)

/**
 * Custom for-each-loop to iterate over all the chunks with valid entities of an entity pool.
 * Just define a block of code just like a for-loop to execute for every chunk with valid entities.
 *
 * @param pool Entity pool to use.
 * @param type Type of the entity.
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index Index of the current chunk.
 * @param iteration_var.entity Pointer to the entity of the chunk.
 *
 * Usage:
 * ```
 * EntityPool pool = EntityPoolCreateType(float);
 *
 * float f;
 * entity_pool_add(&pool, &(f = 1.0f));
 * entity_pool_add(&pool, &(f = 6.7f));
 *
 * entity_pool_for_each_entity(pool, float, itr) {
 *     printf("Iteration: %d. Entity: %.2f [%p]\n", itr.index, *itr.entity, itr.entity");
 * }
 * ```
 */
#define entity_pool_for_each_entity(pool, type, iteration_var) __ILMARTO_ENTITY_POOL_FOR_EACH_ENTITY((pool), type, iteration_var)

#endif // __ILMARTO_ENTITY_POOL_H_
