#pragma once
#ifndef __ILMARTO_ARENA_H_
#define __ILMARTO_ARENA_H_

#include <stddef.h>

typedef struct Arena
{
    void *memory;
    size_t memory_size;
    size_t memory_pos;
} Arena;

/**
 * Creates an arena with the default size.
 * @returns New arena.
 */
Arena arena_create(void);
/**
 * Creates an arena.
 * @param size Size in bytes of the arena.
 * @returns New arena.
 */
Arena areana_create_custom(size_t size);
/**
 * Deletes an arena.
 * @param arena Arena to use.
 */
void areana_delete(Arena *arena);

/**
 * Reserves some bytes on an arena.
 * @param arena Arena to use.
 * @param size Number of bytes to reserve.
 * @returns Pointer to the reserved memory.
 */
void *arena_push(Arena *arena, size_t size);
/**
 * Reserves some bytes for a type on an arena.
 * @param arena Arena to use.
 * @param type Type of the data.
 * @returns Pointer to the reserved memory.
 */
#define arena_push_type(arena, type) (type *)arena_push((arena), sizeof(type))
/**
 * Reserves some bytes on an arena and set them to 0.
 * @param arena Arena to use.
 * @param size Number of bytes to reserve.
 * @returns Pointer to the reserved memory.
 */
void *arena_push_zero(Arena *arena, size_t size);
/**
 * Reserves some bytes for a type on an arena and set them to 0.
 * @param arena Arena to use.
 * @param type Type of the data.
 * @returns Pointer to the reserved memory.
 */
#define arena_push_type_zero(arena, type) (type *)arena_push_zero((arena), sizeof(type))
/**
 * Reserves some bytes for a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 * @returns Pointer to the reserved memory.
 */
#define arena_push_array(arena, type, count) (type *)arena_push((arena), sizeof(type) * (count))
/**
 * Reserves some bytes for a type array on an arena and set them to 0.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 * @returns Pointer to the reserved memory.
 */
#define arena_push_array_zero(arena, type, count) (type *)arena_push_zero((arena), sizeof(type) * (count))

/**
 * Frees some bytes from an arena.
 * @param arena Arena to use.
 * @param size Number of bytes to free.
 */
void arena_pop(Arena *arena, size_t size);
/**
 * Frees some bytes from a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the data.
 */
#define arena_pop_type(arena, type) arena_pop((arena), sizeof(type))
/**
 * Frees some bytes from a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 */
#define arena_pop_array(arena, type, count) arena_pop((arena), sizeof(type) * (count))

/**
 * Gets the used space of an arena.
 * @param arena Arena to use.
 * @returns Used space.
 */
size_t arena_size(Arena arena);

#endif // __ILMARTO_ARENA_H_
