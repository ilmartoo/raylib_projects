#pragma once
#ifndef __SOURCE_STATE_H_
#define __SOURCE_STATE_H_

#include "data_object_pool.h"
#include "entities.h"
#include "raylib.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Font types ------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum FontFamily {
    FONT_ATKINSON_REGULAR = 0,
    FONT_ATKINSON_BOLD,
} FontFamily;

// ----------------------------------------------------------------------------
// ---- Game state ------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct GameState {

    f64 time_elapsed;
    f32 time_delta;

    Vector2 mouse_position;

    DataObjectPool players;
    DataObjectPool enemies;
    DataObjectPool projectiles_players;
    DataObjectPool projectiles_enemies;

    Texture2D spritesheet;
    Rectangle spritesheet_locations_spaceships[4];
    Rectangle spritesheet_locations_proyectiles[2];

    Font fonts[2];
} GameState;

#define SPACESHIP_FRIENDLY_BASE_TEXTURE_POS ((Rectangle) { 320, 0, 96, 96 })
#define SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS ((Rectangle) { 304, 384, 96, 96 })
#define SPACESHIP_ENEMY_BASE_TEXTURE_POS ((Rectangle) { 400, 256, 96, 96 })
#define SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS ((Rectangle) { 512, 0, 96, 96 })

#define PROJECTILE_PLAYER_TEXTURE_POS ((Rectangle) { 608, 0, 64, 127 })
#define PROJECTILE_ENEMY_TEXTURE_POS ((Rectangle) { 688, 160, 64, 127 })

void game_state_initialize(void);
void game_state_clear(void);

Rectangle locate_texture_spaceship(SpaceshipType type); // Spaceship texture location
Rectangle locate_texture_projectile(ProjectileType type); // Proyectile texture location

f32 adjust_to_delta(f32 value);

Font font(FontFamily type);

GameState state = { 0 }; // Game state global variable

#endif // __SOURCE_STATE_H_