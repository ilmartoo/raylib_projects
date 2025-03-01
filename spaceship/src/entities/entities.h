#pragma once
#ifndef __SOURCE_ENTITY_H_
#define __SOURCE_ENTITY_H_

#include "abilities.h"
#include "raylib.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Entity ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Entity
{
    Vector2 position;
    Vector2 size;
    Vector2 movement_speed;
    f32 rotation;
} Entity;

// ----------------------------------------------------------------------------
// ---- Entity types ----------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum SpaceshipType
{
    SPACESHIP_FRIENDLY_BASE = 0,
    SPACESHIP_FRIENDLY_UPGRADED,
    SPACESHIP_ENEMY_BASE,
    SPACESHIP_ENEMY_UPGRADED
} SpaceshipType;

typedef enum ProjectileType
{
    PROYECTILE_PLAYER = 0,
    PROYECTILE_ENEMY,
} ProjectileType;

// ----------------------------------------------------------------------------
// ---- Proyectile ------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Projectile
{
    Entity entity;
    ProjectileType type;

    u32 damage;
    u32 range;
    Vector2 origin;

} Projectile;

Projectile create_projectile(ProjectileType type, Entity entity, u32 damage, u32 range);

// ----------------------------------------------------------------------------
// ---- Player ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Player
{
    Entity entity;
    SpaceshipType type;

    AbilityProjectile shoot;
    AbilityProjectile missile;
} Player;

#define PLAYER_MOVEMENT_SPEED ((Vector2){250, 200})
#define PLAYER_SIZE           ((Vector2){20, 20})

Player create_player(SpaceshipType type);

// ----------------------------------------------------------------------------
// ---- Enemy -----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Enemy
{
    Entity entity;
    SpaceshipType type;

    AbilityProjectile shoot;
} Enemy;

#define ENEMY_SIZE ((Vector2){20, 20})

Enemy create_enemy(SpaceshipType type);

#endif // __SOURCE_ENTITY_H_