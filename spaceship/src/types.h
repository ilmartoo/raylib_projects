#pragma once
#ifndef __ILMARTO_TYPES_H_
#define __ILMARTO_TYPES_H_

#include <stddef.h>  // uintptr_t, ptrdiff_t, size_t
#include <uchar.h>   // char16_t
#include <stdint.h>  // int8_t, int16_t, int32_t, int64_t, uint8_t, uint16_t, uint32_t, uint64_t
#include <stdbool.h> // bool, true, false

typedef char byte; // Byte type

typedef uintptr_t uptr; // Unsigned pointer type

typedef ptrdiff_t size; // Signed size type
typedef size_t usize;   // Unsigned size type

typedef char16_t c16; // 16 bit char type (UTF-16) (Win32: wchar_t)

typedef int8_t i8;   // 8 bit integer type
typedef int16_t i16; // 16 bit integer type
typedef int32_t i32; // 32 bit integer type
typedef int64_t i64; // 64 bit integer type

typedef uint8_t u8;   // 8 bit unsigned integer type
typedef uint16_t u16; // 16 bit unsigned integer type
typedef uint32_t u32; // 32 bit unsigned integer type
typedef uint64_t u64; // 64 bit unsigned integer type

typedef float f32;  // 32 bit float type
typedef double f64; // 64 bit float type

// Retrieves the maximum value
#define max(a, b) (((a) > (b)) ? (a) : (b))
// Retrieves the minimum value
#define min(a, b) (((a) < (b)) ? (a) : (b))

// Allocates a new element in the heap
#define reserve(data_type) (data_type *)malloc(sizeof(data_type))
// Allocates a new array of elements in the heap
#define reserve_array(data_type, number_of_elements) (data_type *)malloc(sizeof(data_type) * number_of_elements)

// Retrieves the size of an array
#define countof(array) (size_t)(sizeof(array) / sizeof(*(array)))
// Retrieves the size of a string
#define lengthof(str) (countof(str) - 1)

#endif // __ILMARTO_TYPES_H_
