#ifndef __FLOAT16_H__
#define __FLOAT16_H__

#include "types.h"

typedef i16 f16;   // IEEE 754 half-precision floating-point
typedef f16 half;  // IEEE 754 half-precision floating-point (alternative notation)

f16 f16_add(f16 a, f16 b);  // Addition of two IEEE 754 half-precision floating-point numbers
f16 f16_sub(f16 a, f16 b);  // Subtraction of two IEEE 754 half-precision floating-point numbers
f16 f16_mul(f16 a, f16 b);  // Multiplication of two IEEE 754 half-precision floating-point numbers
f16 f16_div(f16 a, f16 b);  // Division of two IEEE 754 half-precision floating-point numbers
f16 f16_neg(f16 a);

f16 itof16(i32 i);   // Transformation from integer to half-precision floating point
i32 f16toi(f16 hf);  // Transformation from half-precision floating point to integer

f16 ftof16(f32 f);   // Transformation from single-precision floating point to half-precision floating point
f32 f16tof(f16 hf);  // Transformation from half-precision floating point to single-precision floating point

bool f16_gte(f16 a, f16 b);  // Comparison between two half-precision floating to check if is greater than or equals
bool f16_gt(f16 a, f16 b);   // Comparison between two half-precision floating to check if is greater than
bool f16_lte(f16 a, f16 b);  // Comparison between two half-precision floating to check if is less than or equals
bool f16_lt(f16 a, f16 b);   // Comparison between two half-precision floating to check if is less than
bool f16_eq(f16 a, f16 b);   // Comparison between two half-precision floating to check if is equals
bool f16_neq(f16 a, f16 b);  // Comparison between two half-precision floating to check if is not equals

#endif  // __FLOAT16_H__
