#pragma once
#ifndef __SOURCE_MATHEMATICS_H_
#define __SOURCE_MATHEMATICS_H_

#include "raylib.h"
#include "types.h"

#define PI_QUARTER 0.7853981634f //  45 degs
#define PI_HALF    1.5707963268f //  90 degs
#define PI_3HALFS  4.7123889804f // 270 degs
#define TAU        6.2831853072f // 360 degs

bool CeilNormalizedValue(f32 value);  // Ceils a [0..1] value
bool FloorNormalizedValue(f32 value); // Floors a [0..1] value
bool RoundNormalizedValue(f32 value); // Rounds a [0..1] value

Vector2 Vector2From(f32 x, f32 y);
Vector2 Vector2UnitCirclePoint(f32 rads);

f32 Rad2Deg(f32 rads);
f32 Deg2Rad(f32 degs);

#endif // __SOURCE_MATHEMATICS_H_