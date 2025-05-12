#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include "abilities.h"
#include "actions.h"
#include "raylib.h"

// ----------------------------------------------------------------------------
// ---- Bounding box ----------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct BoundingCircle
{
    f32 radius;
    Vector2 center;

    Vector2 _center_rotated;
} BoundingCircle;

#define BOUNDING_CIRCLE_DEFINITION(x, y, collision_radius, rotation)                                                                       \
    ((BoundingCircle){.radius = collision_radius, .center = {x, y}, ._center_rotated = Vector2Rotate((Vector2){x, y}, rotation)})

// ----------------------------------------------------------------------------
// ---- Entity ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Entity
{
    Vector2 position;               // Top-left position of the entity
    Vector2 size;                   // Size of the entity
    Vector2 movement_speed;         // Movement speed of the entity
    Vector2 velocity;               // Current movement of the entity
    f32 rotation;                   // Rotation of the entity in radians
    f32 _draw_rotation;             // Rotation of the base representation of the  of the entity in radians
    BoundingCircle bounding_circle; // Bounding circle for collision checks
} Entity;

void EntitySetRotation(Entity *entity, f32 rotation);

Vector2 EntityCenter(Entity entity);
Vector2 EntityBoundingCircleCenter(Entity entity);

void EntityDraw(Entity entity, Rectangle texture_location);

// ----------------------------------------------------------------------------
// ---- Health ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Health
{
    u32 current;
    u32 max;
} Health;

#define HEALTH_BAR_HEIGTH       8
#define HEALTH_BAR_BORDER_WIDTH 2
#define HEALTH_BAR_OPACITY      0.5f
#define HEALTH_BAR_SEPARATION   5

#define HEALTH_DEFINITION(max_health) ((Health){max_health, max_health})

void HealthBarDraw(Health health, Entity entity, Color border_color, Color health_color);

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

    Vector2 _origin;
} Projectile;

#define PROJECTILE_DRAW_ROTATION PI_HALF

#define PROJECTILE_BASIC_SIZE_X 5
#define PROJECTILE_BASIC_SIZE_Y 25
#define PROJECTILE_BASIC_SIZE   ((Vector2){PROJECTILE_BASIC_SIZE_X, PROJECTILE_BASIC_SIZE_Y})
#define PROJECTILE_BASIC_SPEED  1200
#define PROJECTILE_BASIC_BOUNDING_CIRCLE(rotation)                                                                                         \
    BOUNDING_CIRCLE_DEFINITION(                                                                                                            \
        0, PROJECTILE_BASIC_SIZE_X - (PROJECTILE_BASIC_SIZE_Y * 0.5f), PROJECTILE_BASIC_SIZE_X, rotation + PROJECTILE_DRAW_ROTATION)

#define PROJECTILE_MISSILE_SIZE_X 8
#define PROJECTILE_MISSILE_SIZE_Y 30
#define PROJECTILE_MISSILE_SIZE   ((Vector2){PROJECTILE_MISSILE_SIZE_X, PROJECTILE_MISSILE_SIZE_Y})
#define PROJECTILE_MISSILE_SPEED  1500
#define PROJECTILE_MISSILE_BOUNDING_CIRCLE(rotation)                                                                                       \
    BOUNDING_CIRCLE_DEFINITION(                                                                                                            \
        0, PROJECTILE_MISSILE_SIZE_X - (PROJECTILE_MISSILE_SIZE_Y * 0.5f), PROJECTILE_MISSILE_SIZE_X, rotation + PROJECTILE_DRAW_ROTATION)

Projectile *ProjectileCreate(ProjectileType type,
                             Vector2 position,
                             Vector2 size,
                             f32 projectile_speed,
                             f32 rotation,
                             BoundingCircle bounding_circle,
                             u32 damage,
                             u32 range);

bool ProjectileMove(Projectile *projectile); // True if the projectile should be eliminated for surpassing maximum range

void ProjectileDraw(Projectile projectile);

// ----------------------------------------------------------------------------
// ---- Player ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Player
{
    Entity entity;
    SpaceshipType type;
    Health health;
    Camera2D camera;

    AbilityRegen ability_regeneration;
    AbilityProjectile ability_shooting;
    AbilityProjectile ability_missile;

    u8 _player_index;
    bool _alternate_shooting;
    InputDevice _device;
} Player;

// Player intrinsics
#define PLAYER_DRAW_ROTATION         PI_HALF
#define PLAYER_MOVEMENT_SPEED        ((Vector2){200, 200})
#define PLAYER_DIAMETER              30
#define PLAYER_SIZE                  ((Vector2){PLAYER_DIAMETER, PLAYER_DIAMETER})
#define PLAYER_HEALTH                HEALTH_DEFINITION(100)
#define PLAYER_CAMERA(pos)           ((Camera2D){.target = pos, .offset = {0}, .rotation = 0, .zoom = 1.f})
#define PLAYER_BOUNDING_CIRCLE_EXTRA 3
#define PLAYER_BOUNDING_CIRCLE(rotation)                                                                                                   \
    BOUNDING_CIRCLE_DEFINITION(0, 2, (PLAYER_DIAMETER * 0.5f) + PLAYER_BOUNDING_CIRCLE_EXTRA, rotation + PLAYER_DRAW_ROTATION)

// Regeneration ability
#define PLAYER_ABILITY_REGENERATION_FLAT                 10
#define PLAYER_ABILITY_REGENERATION_PERCENTAGE           0
#define PLAYER_ABILITY_REGENERATION_DAMAGE_COOLDOWN_TIME 5
#define PLAYER_ABILITY_REGENERATION_REGEN_COOLDOWN_TIME  1
#define PLAYER_ABILITY_REGENERATION                                                                                                        \
    ABILITY_REGEN_DEFINITION(PLAYER_ABILITY_REGENERATION_FLAT,                                                                             \
                             PLAYER_ABILITY_REGENERATION_PERCENTAGE,                                                                       \
                             PLAYER_ABILITY_REGENERATION_DAMAGE_COOLDOWN_TIME,                                                             \
                             PLAYER_ABILITY_REGENERATION_REGEN_COOLDOWN_TIME)

// Shooting ability
#define PLAYER_ABILITY_SHOOT_DAMAGE        10
#define PLAYER_ABILITY_SHOOT_RANGE         1000
#define PLAYER_ABILITY_SHOOT_COOLDOWN_TIME 0.1f
#define PLAYER_ABILITY_SHOOT                                                                                                               \
    ABILITY_PROJECTILE_DEFINITION(PLAYER_ABILITY_SHOOT_DAMAGE, PLAYER_ABILITY_SHOOT_RANGE, PLAYER_ABILITY_SHOOT_COOLDOWN_TIME)

// Missile ability
#define PLAYER_ABILITY_MISSILE_DAMAGE        80
#define PLAYER_ABILITY_MISSILE_RANGE         1000
#define PLAYER_ABILITY_MISSILE_COOLDOWN_TIME 6
#define PLAYER_ABILITY_MISSILE                                                                                                             \
    ABILITY_PROJECTILE_DEFINITION(PLAYER_ABILITY_MISSILE_DAMAGE, PLAYER_ABILITY_MISSILE_RANGE, PLAYER_ABILITY_MISSILE_COOLDOWN_TIME)

#define PLAYER_CANNONS_OFFSET 6

#define PLAYER_ACTION_MOVE_THRESHOLD 0.15f

Player PlayerCreate(InputDevice device, u8 player_index, SpaceshipType type, Vector2 position);

f32 PlayerGetAction(Player player, Action action);

void PlayerMove(Player *player);
void PlayerAim(Player *player);
void PlayerShootBasic(Player *player);
void PlayerShootMissile(Player *player);
void PlayerDamage(Player *player, u32 damage);

void PlayerDraw(Player player);

// ----------------------------------------------------------------------------
// ---- Enemy -----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct Enemy
{
    Entity entity;
    SpaceshipType type;
    Health health;

    AbilityRegen ability_regeneration;
    AbilityProjectile ability_shooting;
} Enemy;

// Enemy intrinsics
#define ENEMY_DRAW_ROTATION         PI_HALF
#define ENEMY_MOVEMENT_SPEED        ((Vector2){100, 100})
#define ENEMY_DIAMETER              35
#define ENEMY_SIZE                  ((Vector2){ENEMY_DIAMETER, ENEMY_DIAMETER})
#define ENEMY_HEALTH                HEALTH_DEFINITION(100)
#define ENEMY_BOUNDING_CIRCLE_EXTRA 7
#define ENEMY_BOUNDING_CIRCLE(rotation)                                                                                                    \
    BOUNDING_CIRCLE_DEFINITION(0, 2, (ENEMY_DIAMETER * 0.5f) + ENEMY_BOUNDING_CIRCLE_EXTRA, rotation + ENEMY_DRAW_ROTATION)

// Regeneration ability
#define ENEMY_ABILITY_REGENERATION_FLAT                 0
#define ENEMY_ABILITY_REGENERATION_PERCENTAGE           10
#define ENEMY_ABILITY_REGENERATION_DAMAGE_COOLDOWN_TIME 10
#define ENEMY_ABILITY_REGENERATION_REGEN_COOLDOWN_TIME  2
#define ENEMY_ABILITY_REGENERATION                                                                                                         \
    ABILITY_REGEN_DEFINITION(ENEMY_ABILITY_REGENERATION_FLAT,                                                                              \
                             ENEMY_ABILITY_REGENERATION_PERCENTAGE,                                                                        \
                             ENEMY_ABILITY_REGENERATION_DAMAGE_COOLDOWN_TIME,                                                              \
                             ENEMY_ABILITY_REGENERATION_REGEN_COOLDOWN_TIME)

// Shooting ability
#define ENEMY_ABILITY_SHOOT_DAMAGE        PLAYER_ABILITY_SHOOT_DAMAGE
#define ENEMY_ABILITY_SHOOT_RANGE         PLAYER_ABILITY_SHOOT_RANGE
#define ENEMY_ABILITY_SHOOT_COOLDOWN_TIME (PLAYER_ABILITY_SHOOT_COOLDOWN_TIME * 2)
#define ENEMY_ABILITY_SHOOT                                                                                                                \
    ABILITY_PROJECTILE_DEFINITION(ENEMY_ABILITY_SHOOT_DAMAGE, ENEMY_ABILITY_SHOOT_RANGE, ENEMY_ABILITY_SHOOT_COOLDOWN_TIME)

Enemy *EnemyCreate(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation);

bool EnemyDamage(Enemy *enemy, u32 damage); // True if the enemy should be eliminated

void EnemyDraw(Enemy enemy);

#endif // ENTITY_H