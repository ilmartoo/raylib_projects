#include <stdlib.h>

#include "memory_utils.h"
#include "object_pool.h"
#include "types.h"

usize __DuplicateToReachTarget(usize num, usize target)
{
    while (num < target) { num *= 2; }
    return num;
}

void *__UpdateObjectFromObjectPool(void *object_pos, const void *const new_value, usize object_size)
{
    memory_copy(object_pos, new_value, object_size);
    *((bool *)object_pos + object_size) = true;
    return object_pos;
}

ObjectPool ObjectPoolCreate(usize object_size) { return ObjectPoolCreateCustom(object_size, __ILMARTO_DATA_OBJECT_POOL_DEFAULT_MEM_SIZE); }

ObjectPool ObjectPoolCreateCustom(usize object_size, usize initial_mem_size)
{
    return (ObjectPool){.chunk_count = 0,
                        .object_count = 0,
                        .object_size = object_size,
                        .chunks = (byte *)malloc(initial_mem_size),
                        .chunk_size = object_size + sizeof(bool),
                        .mem_size = initial_mem_size};
}

void ObjectPoolDelete(ObjectPool *pool) { free(pool->chunks); }

void *ObjectPoolAdd(ObjectPool *pool, const void *const data)
{
    ForEachObjectPoolChunk(pool, void, iteration)
    {
        if (!iteration.valid)
        {
            __UpdateObjectFromObjectPool(iteration.object, data, pool->object_size);
            ++pool->object_count;
            return iteration.object;
        }
    }

    usize current_size = pool->chunk_count * pool->chunk_size;
    usize needed_size = pool->chunk_size + current_size;

    if (pool->mem_size < needed_size)
    {
        pool->mem_size = __DuplicateToReachTarget(pool->mem_size, needed_size);
        pool->chunks = (byte *)realloc(pool->chunks, pool->mem_size);
    }

    void *last_data_object = pool->chunks + current_size;
    __UpdateObjectFromObjectPool(last_data_object, data, pool->object_size);
    pool->object_count = ++pool->chunk_count;
    return last_data_object;
}

void ObjectPoolPop(ObjectPool *pool, u32 object_index)
{
    bool *is_used = (bool *)pool->chunks + (pool->chunk_size * object_index) + pool->object_size;
    if (*is_used)
    {
        *is_used = false;
        --pool->object_count;
    }
}

void *ObjectPoolObjectAt(ObjectPool pool, u32 object_index)
{
    void *data_object = pool.chunks + (pool.chunk_size * object_index);
    return *((bool *)data_object + pool.object_size) ? data_object : NULL;
}

u32 ObjectPoolObjectCount(ObjectPool pool) { return pool.object_count; }

u32 ObjectPoolChunkCount(ObjectPool pool) { return pool.chunk_count; }
