#include <math.h>

#include "debug.h"
#include "float.h"
#include "mathematics.h"
#include "raylib.h"
#include "types.h"

bool unit_ceil(f32 value) { return value > 0 ? 1 : 0; }

bool unit_floor(f32 value) { return 1.f < value ? 0 : 1; }

bool unit_round(f32 value) { return value >= 0.5f ? 1 : 0; }

Vector2 vector2_unit_at_angle(f32 radians) { return (Vector2){cosf(radians), sinf(radians)}; }

f32 rad_to_deg(f32 radians) { return radians * RAD2DEG; }

f32 deg_to_rad(f32 degrees) { return degrees * DEG2RAD; }
