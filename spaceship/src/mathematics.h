#pragma once
#ifndef __SOURCE_MATHEMATICS_H_
#define __SOURCE_MATHEMATICS_H_

#include "raylib.h"
#include "types.h"

bool unit_ceil(f32 value);  // Ceils a [0..1] value
bool unit_floor(f32 value); // Floors a [0..1] value
bool unit_round(f32 value); // Rounds a [0..1] value

Vector2 vector2_unit_at_angle(f32 radians);

f32 rad_to_deg(f32 radians);
f32 deg_to_rad(f32 degrees);

#endif // __SOURCE_MATHEMATICS_H_