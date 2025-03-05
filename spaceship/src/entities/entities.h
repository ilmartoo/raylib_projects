#pragma once
#ifndef __SOURCE_ENTITY_H_
#define __SOURCE_ENTITY_H_

#include "abilities.h"
#include "inputs.h"
#include "raylib.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Entity ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Entity
{
    Vector2 position;       // Top-left position of the entity
    Vector2 size;           // Size of the entity
    Vector2 movement_speed; // Movement speed of the entity
    Vector2 velocity;       // Current movement of the entity
    f32 rotation;           // Rotation of the entity in radians
} Entity;

Vector2 entity_center(Entity entity);

#ifdef DEBUG
void draw_entity_bounding_box(Entity entity, Color color);
#endif

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

#define PROJECTILE_ROTATION (PI * -0.5)

#define PROJECTILE_STANDARD_SIZE  ((Vector2){5, 15})
#define PROJECTILE_STANDARD_SPEED 500

#define PROJECTILE_MISSILE_SIZE  ((Vector2){8, 25})
#define PROJECTILE_MISSILE_SPEED 700

Projectile *projectile_create(ProjectileType type, Entity entity, u32 damage, u32 range);

bool projectile_move(Projectile *projectile);

void proyectile_draw(Projectile projectile);

// ----------------------------------------------------------------------------
// ---- Player ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Player
{
    Entity entity;
    SpaceshipType type;

    Vector2 aiming_at;
    bool shoot_from_left;

    bool using_gamepad;
    PlayerControls controls[2];

    AbilityProjectile shoot;
    AbilityProjectile shoot_missile;
} Player;

#define PLAYER_ROTATION       (PI)
#define PLAYER_MOVEMENT_SPEED ((Vector2){250, 200})
#define PLAYER_SIZE           ((Vector2){20, 20})

#define PLAYER_CANNONS_OFFSET 6

#define PLAYER_AIMING_SIGHT_MAX_DISTANCE 200
#define PLAYER_AIMING_SIGHT_MIN_DISTANCE 30
#define PLAYER_AIMING_SIGHT_SIZE         ((Vector2){10, 10})
#define PLAYER_AIMING_SIGHT_THICKNESS    2

#define PLAYER_THRESHOLD_MOVEMENT 0.15

Player *player_create(SpaceshipType type, Vector2 position);

f32 player_input(Player player, PlayerAction action);

void player_move(Player *player);
void player_aim(Player *player);

void player_shoot(Player *player);
void player_shoot_missile(Player *player);

void player_draw(Player player);

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

Enemy *enemy_create(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation);

void enemy_draw(Enemy enemy);

#endif // __SOURCE_ENTITY_H_