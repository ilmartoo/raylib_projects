#include "entities.h"
#include "abilities.h"

Projectile projectile_create(ProjectileType type, Entity entity, u32 damage, u32 range)
{
    return (Projectile) {
        .type = type,
        .entity = entity,
        .damage = damage,
        .range = range,
        .origin = entity.position,
    };
}

Player player_create(SpaceshipType type)
{
    return (Player) { .entity = { .position = { 0 }, .size = PLAYER_SIZE, .movement_speed = PLAYER_MOVEMENT_SPEED, .rotation = 0 },
        .type = type,
        .shoot = ABILITY_SHOOT,
        .missile = ABILITY_MISSILE };
}

Enemy enemy_create(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation)
{
    return (Enemy) { .entity = { .position = position, .size = ENEMY_SIZE, .movement_speed = movement_speed, .rotation = rotation },
        .type = type,
        .shoot = ABILITY_SHOOT };
}
