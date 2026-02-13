#pragma once
#ifndef MATHEMATICS_H
#define MATHEMATICS_H

#include "raylib/raylib.h"
#include "types/types.h"

#define PI_QUARTER 0.7853981634f  //  45 degs
#define PI_HALF    1.5707963268f  //  90 degs
#define PI_3HALFS  4.7123889804f  // 270 degs
#define TAU        6.2831853072f  // 360 degs

#define MAX_DEGS 360  // Degs in a circle

bool CeilNormalizedValue(f32 value);   // Ceils a [0..1] value
bool FloorNormalizedValue(f32 value);  // Floors a [0..1] value
bool RoundNormalizedValue(f32 value);  // Rounds a [0..1] value

Vector2 Vector2From(f32 x, f32 y);

Vector2 Vector2UnitCirclePoint(f32 rads);
Vector2 Vector2RotateFrom(Vector2 v, Vector2 center, f32 rads);
f32 Vector2AngleFromXAxis(Vector2 v);
f32 Vector2AngleFromYAxis(Vector2 v);

f32 Rad2Deg(f32 rads);
f32 Deg2Rad(f32 degs);

#endif  // MATHEMATICS_H