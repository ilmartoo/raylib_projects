#include <stdlib.h>
#include "entity_pool.h"
#include "types.h"
#include "custom_memory.h"

EntityPool entity_pool_create(usize entity_size) { return entity_pool_create_custom(entity_size, __ILMARTO_ENTITY_POOL_DEFAULT_MEM_SIZE); }

EntityPool entity_pool_create_custom(usize entity_size, usize initial_mem_size)
{
    return (EntityPool){.chunk_count = 0,
                        .entity_count = 0,
                        .entity_size = entity_size,
                        .chunks = (byte *)malloc(initial_mem_size),
                        .chunk_size = entity_size + sizeof(bool),
                        .mem_size = initial_mem_size};
}

void entity_pool_release(EntityPool *pool) { free(pool->chunks); }

void *entity_pool_add(EntityPool *pool, const void *const entity)
{
    entity_pool_for_each_chunk(pool, void, iteration)
    {
        bool *is_used = (bool *)((uptr)iteration.entity + pool->entity_size);
        if (!*is_used)
        {
            *is_used = true;
            void *new_entity = memory_copy(iteration.entity, entity, pool->entity_size);
            ++pool->entity_count;

            return new_entity;
        }
    }

    usize current_size = pool->chunk_count * pool->chunk_size;
    usize mem_size = pool->mem_size;
    if (mem_size - current_size < pool->chunk_size)
    {
        do {
            mem_size *= 2;
        } while (mem_size - current_size < pool->chunk_size);
        pool->chunks = (byte *)realloc(pool->chunks, mem_size);
        pool->mem_size = mem_size;
    }

    void *new_entity = memory_copy(pool->chunks + current_size, entity, pool->entity_size);
    *((bool *)new_entity + pool->entity_size) = true;
    pool->entity_count = pool->chunk_count += 1;

    return new_entity;
}

void entity_pool_pop(EntityPool *pool, u32 entity_index)
{
    bool *is_used = (bool *)pool->chunks + (pool->chunk_size * entity_index) + pool->entity_size;
    if (*is_used)
    {
        *is_used = false;
        --pool->entity_count;
    }
}

void *entity_pool_entity_at(EntityPool pool, u32 entity_index)
{
    if (entity_index < pool.chunk_count)
    {
        void *entity = pool.chunks + (pool.chunk_size * entity_index);
        return *((bool *)entity + pool.entity_size) ? entity : NULL;
    }
    return NULL;
}

u32 entity_pool_entity_count(EntityPool pool) { return pool.entity_count; }

u32 entity_pool_chunk_count(EntityPool pool) { return pool.chunk_count; }
