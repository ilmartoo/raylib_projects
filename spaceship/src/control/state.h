#pragma once
#ifndef __SOURCE_STATE_H_
#define __SOURCE_STATE_H_

#include "object_pool.h"
#include "entities.h"
#include "rayheader.h"

// ----------------------------------------------------------------------------
// ---- Font types ------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum FontFamily
{
    FONT_ATKINSON_REGULAR = 0,
    FONT_ATKINSON_BOLD,
} FontFamily;

// ----------------------------------------------------------------------------
// ---- Game state ------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct GameState
{
    f64 time_elapsed;
    f32 time_delta_real;
    f32 time_delta_simulation;
    i16 time_speed_magnitude;
    bool time_running;

    ObjectPool input_profiles;

    ObjectPool players;
    ObjectPool enemies;
    ObjectPool projectiles_players;
    ObjectPool projectiles_enemies;

    Texture2D spritesheet;
    Rectangle spritesheet_locations_spaceships[4];
    Rectangle spritesheet_locations_proyectiles[2];

    Font fonts[2];
} GameState;

#define GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX 5

#define TEXTURE_POS_SPACESHIP_FRIENDLY_BASE     ((Rectangle){320, 0, 96, 96})
#define TEXTURE_POS_SPACESHIP_FRIENDLY_UPGRADED ((Rectangle){304, 384, 96, 96})
#define TEXTURE_POS_SPACESHIP_ENEMY_BASE        ((Rectangle){400, 256, 96, 96})
#define TEXTURE_POS_SPACESHIP_ENEMY_UPGRADED    ((Rectangle){512, 0, 96, 96})

#define TEXTURE_POS_PROJECTILE_PLAYER ((Rectangle){608, 0, 64, 127})
#define TEXTURE_POS_PROJECTILE_ENEMY  ((Rectangle){688, 160, 64, 127})

void GameStateInitialization(void);
void GameStateUpdate(void); // Call only if the state is initialized
void GameStateCleanup(void);

Rectangle SpaceshipTextureLocation(SpaceshipType type);   // Spaceship texture location
Rectangle ProjectileTextureLocation(ProjectileType type); // Proyectile texture location

f32 ScaleToDelta(f32 value);
Vector2 Vector2ScaleToDelta(Vector2 v);

Font FontFromFamily(FontFamily type);

extern GameState *state; // Game state global variable

#endif // __SOURCE_STATE_H_