#include <stdlib.h>

#include "arena.h"

#define DEFAULT_ARENA_PAGE_BYTE_SIZES 1024

Arena ArenaCreateCustom(size_t size) { return (Arena){malloc(size), size, 0}; }

Arena ArenaCreate(void) { return ArenaCreateCustom(DEFAULT_ARENA_PAGE_BYTE_SIZES); }

void ArenaDelete(Arena *arena) { free(arena->memory); };

void *ArenaPush(Arena *arena, size_t size)
{
    if (arena->memory_size - arena->memory_pos < size)
    {
        do {
            arena->memory_size *= 2;
        } while (arena->memory_size - arena->memory_pos < size);
        arena->memory = realloc(arena->memory, arena->memory_size);
    }
    void *reserved_memory = (void *)((uintptr_t)arena->memory + arena->memory_pos);
    arena->memory_pos += size;
    return reserved_memory;
}

void *ArenaPushZero(Arena *arena, size_t size)
{
    void *reserved_memory = ArenaPush(arena, size);
    for (char *m = reserved_memory; size; --size) { *(m++) = 0; }
    return reserved_memory;
}

void ArenaPop(Arena *arena, size_t size) { arena->memory_pos = arena->memory_pos < size ? 0 : arena->memory_pos - size; }

void ArenaClear(Arena *arena) { arena->memory_pos = 0; }

size_t ArenaSize(Arena arena) { return arena.memory_pos; }
