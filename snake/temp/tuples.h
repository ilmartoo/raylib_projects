#pragma once
#ifndef _TUPLES_H_
#define _TUPLES_H_

#include "types.h"

// i64 tuple
typedef struct TupleI64 {
    i64 x;
    i64 y;
} TupleI64;

// i32 tuple
typedef struct TupleI32 {
    i32 x;
    i32 y;
} TupleI32;

// u64 tuple
typedef struct TupleU64 {
    u64 x;
    u64 y;
} TupleU64;

// u32 tuple
typedef struct TupleU32 {
    u32 x;
    u32 y;
} TupleU32;

#endif //_TUPLES_H_
