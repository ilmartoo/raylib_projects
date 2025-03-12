#pragma once
#ifndef __SOURCE_ENTITY_H_
#define __SOURCE_ENTITY_H_

#include "abilities.h"
#include "player_actions.h"
#include "rayheader.h"

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
    f32 draw_rotation;      // Rotation of the base representation of the  of the entity in radians
} Entity;

Vector2 EntityCenter(Entity entity);

void EntityDraw(Entity entity, Rectangle texture_location);

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

#define PROJECTILE_DRAW_ROTATION PI_HALF

#define PROJECTILE_BASIC_SIZE  ((Vector2){5, 15})
#define PROJECTILE_BASIC_SPEED 600

#define PROJECTILE_MISSILE_SIZE  ((Vector2){8, 25})
#define PROJECTILE_MISSILE_SPEED 750

Projectile *ProjectileCreate(ProjectileType type, Entity entity, u32 damage, u32 range);

bool ProjectileMove(Projectile *projectile);

void ProjectileDraw(Projectile projectile);

// ----------------------------------------------------------------------------
// ---- Player ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Player
{
    Entity entity;
    SpaceshipType type;

    bool alternate_shooting;

    InputDevice input_device;

    AbilityProjectile shooting;
    AbilityProjectile shooting_missile;
} Player;

#define PLAYER_DRAW_ROTATION  PI_HALF
#define PLAYER_MOVEMENT_SPEED ((Vector2){250, 200})
#define PLAYER_DIAMETER       30
#define PLAYER_SIZE           ((Vector2){PLAYER_DIAMETER, PLAYER_DIAMETER})

#define PLAYER_CANNONS_OFFSET 6

// #define PLAYER_AIMING_SIGHT_MAX_DISTANCE 200
// #define PLAYER_AIMING_SIGHT_MIN_DISTANCE (PLAYER_DIAMETER + 10)
// #define PLAYER_AIMING_SIGHT_SIZE         ((Vector2){16, 10})
// #define PLAYER_AIMING_SIGHT_THICKNESS    2

#define PLAYER_ACTION_MOVE_THRESHOLD 0.15f

Player *PlayerCreate(SpaceshipType type, Vector2 position);

f32 PlayerCheckAction(Player *player, PlayerAction action);

void PlayerMove(Player *player);
void PlayerAim(Player *player);

void PlayerShootBasic(Player *player);
void PlayerShootMissile(Player *player);

void PlayerDraw(Player player);

// ----------------------------------------------------------------------------
// ---- Enemy -----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Enemy
{
    Entity entity;
    SpaceshipType type;

    AbilityProjectile shooting;
} Enemy;

#define ENEMY_SIZE ((Vector2){20, 20})

Enemy *EnemyCreate(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation);

void EnemyDraw(Enemy enemy);

#endif // __SOURCE_ENTITY_H_