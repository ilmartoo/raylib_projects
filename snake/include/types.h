#pragma once
#ifndef _TYPES_H_
#define _TYPES_H_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <uchar.h>

// 8 bit unsigned integer type
typedef uint8_t u8;
// 16 bit char type (UTF-16) (Win32: wchar_t)
typedef char16_t c16;
// 32 bit unsigned integer type
typedef int32_t i32;
// 64 bit unsigned integer type
typedef int64_t i64;
// 32 bit unsigned integer type
typedef uint32_t u32;
// 64 bit unsigned integer type
typedef uint64_t u64;
// 32 bit float type
typedef float f32;
// 64 bit float type
typedef double f64;
// Unsigned pointer type
typedef uintptr_t uptr;
// Byte type
typedef char byte;
// Signed size type
typedef ptrdiff_t size;
// Unsigned size type
typedef size_t usize;
// String type
typedef char* string;

#if not defined(__cplusplus) && not defined(bool)

    // 32 bit boolean type
    typedef int32_t bool;
    // True boolean value
    #define true 1
    // False boolean value
    #define false 0

#endif // bool

#endif // _TYPES_H_
