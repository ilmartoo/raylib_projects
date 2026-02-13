/**
 * The IEEE 754 half-precision floating-point number implementation
 * was created to represent decimal numbers in a small memory space.
 *
 * The implementation allows for a precision of 3 digits maximum, with
 * notable precision errors if surpassed.
 *
 * It is useful to store aproximations of floating-point numbers in
 * big groups to reduce bandwidth and/or increment transfer speeds.
 */
#pragma once
#ifndef __FLOAT16_H__
#define __FLOAT16_H__

#include <stdint.h>

typedef int16_t half;  // IEEE 754 half-precision floating-point number
typedef int16_t f16;   // IEEE 754 half-precision floating-point number

#define HLF_EPSILON 9.77e-04F  // Smallest number such that 1.0+HLF_EPSILON != 1.0

half f16_add(half h1, half h2);  // Addition of two IEEE 754 half-precision floating-point numbers
half f16_sub(half h1, half h2);  // Subtraction of two IEEE 754 half-precision floating-point numbers
half f16_mul(half h1, half h2);  // Multiplication of two IEEE 754 half-precision floating-point numbers
half f16_div(half h1, half h2);  // Division of two IEEE 754 half-precision floating-point numbers
half f16_neg(half h);            // Negates a IEEE 754 half-precision floating-point number

half itof16(int32_t i);  // Transformation from integer to half-precision floating point
int32_t f16toi(half h);  // Transformation from half-precision floating point to integer

half ftof16(float f);  // Transformation from single-precision floating point to half-precision floating point
float f16tof(half h);  // Transformation from half-precision floating point to single-precision floating point

int f16_eq(half h1, half h2);   // Comparison between two half-precision floating to check if is equals
int f16_neq(half h1, half h2);  // Comparison between two half-precision floating to check if is not equals
int f16_gt(half h1, half h2);   // Comparison between two half-precision floating to check if is greater than
int f16_gte(half h1, half h2);  // Comparison between two half-precision floating to check if is greater than or equals
int f16_lt(half h1, half h2);   // Comparison between two half-precision floating to check if is less than
int f16_lte(half h1, half h2);  // Comparison between two half-precision floating to check if is less than or equals

#endif  // __FLOAT16_H__
