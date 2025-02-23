#pragma once
#ifndef __ILMARTO_MEMORY_H_
#define __ILMARTO_MEMORY_H_

#include <stddef.h>

/**
 * Copies the specified number of bytes from a source to a destination.
 * @param dest Destination memory space.
 * @param src Source memory space.
 * @param bytes Number of bytes to copy.
 * @return Reference to the destination memory space.
 */
void *memory_copy(void *dest, const void *const src, size_t bytes);
/**
 * Copies the bytes of a type from a source to a destination.
 * @param dest Destination memory space.
 * @param src Source memory space.
 * @param type Type to copy.
 * @return Reference to the destination memory space.
 */
#define memory_copy_type(dest, src, type) ((type *)memory_copy(dest, src, sizeof(type)))

/**
 * Sets the specified number of bytes from a memory space to 0.
 * @param memory Memory space.
 * @param bytes Number of bytes update.
 * @return Reference to the memory space.
 */
void *memory_zero(void *memory, size_t bytes);
/**
 * Sets the type contained in the memory space to 0.
 * @param memory Memory space.
 * @param type Type to set to 0.
 * @return Reference to the memory space.
 */
#define memory_zero_type(memory, type) ((type *)memory_zero(memory, sizeof(type)))

#endif // __ILMARTO_MEMORY_H_
