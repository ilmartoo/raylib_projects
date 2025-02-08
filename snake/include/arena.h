#pragma once
#ifndef _ARENA_H_
#define _ARENA_H_

#include "types.h"

typedef struct Arena Arena;

/**
 * @brief Creates an arena.
 * @param size Size in bytes of the arena.
 * @returns New arena.
 */
Arena ArenaAlloc(usize size);
/**
 * @brief Creates an arena with the default size.
 * @returns New arena.
 */
Arena ArenaDefaultAlloc(void);
/**
 * @brief Deletes an arena.
 * @param arena Arena to use.
 */
void ArenaRelease(Arena *arena);

/**
 * @brief Reserves some bytes on an arena.
 * @param arena Arena to use.
 * @param size Number of bytes to reserve.
 * @returns Pointer to the reserved memory.
 */
void *ArenaPush(Arena *arena, usize size);
/**
 * @brief Reserves some bytes on an arena and set them to 0.
 * @param arena Arena to use.
 * @param size Number of bytes to reserve.
 * @returns Pointer to the reserved memory.
 */
void *ArenaPushZero(Arena *arena, usize size);
/**
 * @brief Reserves some bytes for a type on an arena.
 * @param arena Arena to use.
 * @param type Type of the data.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushType(arena, type) ArenaPush((arena), sizeof(type))
/**
 * @brief Reserves some bytes for a type on an arena and set them to 0.
 * @param arena Arena to use.
 * @param type Type of the data.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushTypeZero(arena, type) ArenaPushZero((arena), sizeof(type))
/**
 * @brief Reserves some bytes for a type array on an arena.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushArray(arena, type, count) (type *)ArenaPush((arena), sizeof(type)*(count))
/**
 * @brief Reserves some bytes for a type array on an arena and set them to 0.
 * @param arena Arena to use.
 * @param type Type of the array's data.
 * @param count Size of the array.
 * @returns Pointer to the reserved memory.
 */
#define ArenaPushArrayZero(arena, type, count) (type *)ArenaPushZero((arena), sizeof(type)*(count))

/**
 * @brief Frees some bytes from an arena.
 * @param arena Arena to use.
 */
void ArenaPop(Arena *arena, usize size);
/**
 * @brief Frees all bytes from an arena.
 * @param arena Arena to use.
 */
void ArenaClear(Arena *arena);

/**
 * @brief Gets the used space of an arena.
 * @param arena Arena to use.
 * @returns Used space.
 */
usize ArenaUsedSpace(Arena *arena);

#endif // _ARENA_H_
