#include <math.h>

#include "debug.h"
#include "float.h"
#include "mathematics.h"
#include "raylib.h"
#include "types.h"

bool CeilNormalizedValue(f32 value) { return value > 0 ? 1 : 0; }

bool FloorNormalizedValue(f32 value) { return 1.f < value ? 0 : 1; }

bool RoundNormalizedValue(f32 value) { return value >= 0.5f ? 1 : 0; }

Vector2 Vector2From(f32 x, f32 y) { return (Vector2){x, y}; }

Vector2 Vector2UnitCirclePoint(f32 rads) { return (Vector2){cosf(rads), sinf(rads)}; }

f32 Rad2Deg(f32 rads) { return rads * RAD2DEG; }

f32 Deg2Rad(f32 degs) { return degs * DEG2RAD; }
