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
Arena ArenaCreate(void);
/**
 * Creates an arena.
 * @param size Size in bytes of the arena.
 * @returns New arena.
 */
Arena ArenaCreateCustom(size_t size);
/**
 * Deletes an arena.
 * @param arena Arena to use.
 */
void ArenaDelete(Arena *arena);

/**
 * Reserves some bytes on an arena.
 * @param arena Arena to use.
 * @param size Number of bytes to reserve.
 * @returns Pointer to the reserved memory.
 */
void *ArenaPush(Arena *arena, size_t size);
/**
 * Reserves some bytes for a type on an arena.
 * @param arena Arena to use.
 * @param type Type of the data.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushType(arena, type) (type *)ArenaPush((arena), sizeof(type))
/**
 * Reserves some bytes on an arena and set them to 0.
 * @param arena Arena to use.
 * @param size Number of bytes to reserve.
 * @returns Pointer to the reserved memory.
 */
void *ArenaPushZero(Arena *arena, size_t size);
/**
 * Reserves some bytes for a type on an arena and set them to 0.
 * @param arena Arena to use.
 * @param type Type of the data.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushTypeZero(arena, type) (type *)ArenaPushZero((arena), sizeof(type))
/**
 * Reserves some bytes for a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushArray(arena, type, count) (type *)ArenaPush((arena), sizeof(type) * (count))
/**
 * Reserves some bytes for a type array on an arena and set them to 0.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushArrayZero(arena, type, count) (type *)ArenaPushZero((arena), sizeof(type) * (count))

/**
 * Frees some bytes from an arena.
 * @param arena Arena to use.
 * @param size Number of bytes to free.
 */
void ArenaPop(Arena *arena, size_t size);
/**
 * Frees some bytes from a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the data.
 */
#define ArenaPopType(arena, type) ArenaPop((arena), sizeof(type))
/**
 * Frees some bytes from a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 */
#define ArenaPopArray(arena, type, count) ArenaPop((arena), sizeof(type) * (count))
/**
 * Frees all bytes from an arena.
 * @param arena Arena to use.
 */
void ArenaClear(Arena *arena);

/**
 * Gets the used space of an arena.
 * @param arena Arena to use.
 * @returns Used space.
 */
size_t ArenaSize(Arena arena);

#endif // __ILMARTO_ARENA_H_
