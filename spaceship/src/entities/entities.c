#include "entities.h"
#include "abilities.h"

Projectile create_projectile(ProjectileType type, Entity entity, u32 damage, u32 range)
{
    return (Projectile){
        .type = type,
        .entity = entity,
        .damage = damage,
        .range = range,
        .origin = entity.position,
    };
}

Player create_player(SpaceshipType type)
{
    return (Player){.entity = {.position = {0}, .size = PLAYER_SIZE, .movement_speed = PLAYER_MOVEMENT_SPEED, .rotation = 0},
                    .type = type,
                    .shoot = ABILITY_SHOOT,
                    .missile = ABILITY_MISSILE};
}

Enemy create_enemy(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation)
{
    return (Enemy){.entity = {.position = position, .size = ENEMY_SIZE, .movement_speed = movement_speed, .rotation = rotation},
                   .type = type,
                   .shoot = ABILITY_SHOOT};
}
