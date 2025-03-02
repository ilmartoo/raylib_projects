#include <stdlib.h>

#include "data_object_pool.h"
#include "memory_utils.h"
#include "types.h"

usize duplicate_until_enough(usize num, usize needed)
{
    while (num < needed) {
        num *= 2;
    }
    return num;
}

void* update_data_object(void* data_object_pos, const void* const new_value, usize data_object_size)
{
    memory_copy(data_object_pos, new_value, data_object_size);
    *((bool*)data_object_pos + data_object_size) = true;
    return data_object_pos;
}

DataObjectPool data_object_pool_create(usize data_object_size)
{
    return data_object_pool_create_custom(data_object_size, __ILMARTO_DATA_OBJECT_POOL_DEFAULT_MEM_SIZE);
}

DataObjectPool data_object_pool_create_custom(usize data_object_size, usize initial_mem_size)
{
    return (DataObjectPool) { .chunk_count = 0,
        .data_object_count = 0,
        .data_object_size = data_object_size,
        .chunks = (byte*)malloc(initial_mem_size),
        .chunk_size = data_object_size + sizeof(bool),
        .mem_size = initial_mem_size };
}

void data_object_pool_release(DataObjectPool* pool) { free(pool->chunks); }

void* data_object_pool_add(DataObjectPool* pool, const void* const data_object)
{
    data_object_pool_for_each_chunk(pool, void, iteration)
    {
        if (!iteration.valid) {
            update_data_object(iteration.data, data_object, pool->data_object_size);
            ++pool->data_object_count;
            return iteration.data;
        }
    }

    usize current_size = pool->chunk_count * pool->chunk_size;
    usize needed_size = pool->chunk_size + current_size;

    if (pool->mem_size < needed_size) {
        pool->mem_size = duplicate_until_enough(pool->mem_size, needed_size);
        pool->chunks = (byte*)realloc(pool->chunks, pool->mem_size);
    }

    void* last_data_object = pool->chunks + current_size;
    update_data_object(last_data_object, data_object, pool->data_object_size);
    pool->data_object_count = ++pool->chunk_count;
    return last_data_object;
}

void data_object_pool_pop(DataObjectPool* pool, u32 data_object_index)
{
    bool* is_used = (bool*)pool->chunks + (pool->chunk_size * data_object_index) + pool->data_object_size;
    if (*is_used) {
        *is_used = false;
        --pool->data_object_count;
    }
}

void* data_object_pool_object_at(DataObjectPool pool, u32 data_object_index)
{
    void* data_object = pool.chunks + (pool.chunk_size * data_object_index);
    return *((bool*)data_object + pool.data_object_size) ? data_object : NULL;
}

u32 data_object_pool_object_count(DataObjectPool pool) { return pool.data_object_count; }

u32 data_object_pool_chunk_count(DataObjectPool pool) { return pool.chunk_count; }
