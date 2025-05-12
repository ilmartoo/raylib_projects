#include <math.h>

#include "extra_math.h"
#include "float.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"

bool CeilNormalizedValue(f32 value) { return value > 0 ? 1 : 0; }

bool FloorNormalizedValue(f32 value) { return 1 < value ? 0 : 1; }

bool RoundNormalizedValue(f32 value) { return value >= 0.5f ? 1 : 0; }

Vector2 Vector2From(f32 x, f32 y) { return (Vector2){x, y}; }

Vector2 Vector2UnitCirclePoint(f32 rads) { return (Vector2){cosf(rads), sinf(rads)}; }

Vector2 Vector2RotateFrom(Vector2 v, Vector2 center, f32 rads)
{
    return Vector2Add(center, Vector2Rotate(Vector2Subtract(v, center), rads));
}

f32 Vector2AngleFromXAxis(Vector2 v) { return Vector2Angle(Vector2From(1, 0), v); }

f32 Vector2AngleFromYAxis(Vector2 v) { return Vector2Angle(Vector2From(0, 1), v); }

f32 Rad2Deg(f32 rads) { return rads * RAD2DEG; }

f32 Deg2Rad(f32 degs) { return degs * DEG2RAD; }
