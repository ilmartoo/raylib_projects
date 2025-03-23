#include "collisions.h"
#include "entities.h"
#include "rayheader.h"

bool CheckEntityCollision(Entity e1, Entity e2)
{
    return Vector2Distance(EntityBoundingCircleCenter(e1), EntityBoundingCircleCenter(e2)) <=
           (e1.bounding_circle.radius + e2.bounding_circle.radius);
}