#include <stdlib.h>
#include <string.h>
#include "arena.h"

#define DEFAULT_ARENA_PAGE_BYTE_SIZES 1024

Arena ArenaCreateCustom(size_t size) { return (Arena) { malloc(size), size, 0 }; }

Arena ArenaCreate(void) { return ArenaCreateCustom(DEFAULT_ARENA_PAGE_BYTE_SIZES); }

void ArenaRelease(Arena* arena) { free(arena->stack); };

void* ArenaPush(Arena* arena, size_t size)
{
    if (arena->size - arena->pos < size) {
        do {
            arena->size *= 2;
        } while (arena->size - arena->pos < size);
        arena->stack = realloc(arena->stack, arena->size);
    }
    void* allocPos = (void*)((size_t)arena->stack + arena->pos);
    arena->pos += size;
    return allocPos;
}

void* ArenaPushZero(Arena* arena, size_t size)
{
    void* allocPos = ArenaPush(arena, size);
    memset(allocPos, 0, size);
    return allocPos;
}

void ArenaPop(Arena* arena, size_t size) { arena->pos = arena->pos < size ? 0 : arena->pos - size; }

void ArenaClear(Arena* arena) { arena->pos = 0; }

size_t ArenaUsedSpace(Arena* arena) { return arena->pos; }
