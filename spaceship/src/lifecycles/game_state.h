#pragma once
#ifndef __STATE_H__
#define __STATE_H__

#include "config.h"
#include "entities.h"
#include "input-handler.h"
#include "object_pool.h"
#include "raylib.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Game state ------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef struct GameState {
    /* Time */
    f64 time_elapsed;
    f32 time_delta_real;
    f32 time_delta_simulation;
    i16 time_speed_magnitude;
    bool time_running;

    /* Player */
    Player player;
    bool game_over;

    /* Inputs */
    GreedyInputHandler input_handler;

    /* Entity pools */
    ObjectPool enemies;
    ObjectPool projectiles_players;
    ObjectPool projectiles_enemies;

    /* Textures */
    Texture2D spritesheet;
    Rectangle spritesheet_locations_spaceships[4];
    Rectangle spritesheet_locations_proyectiles[2];

    /* Fonts */
    Font font;

    /* Testing */
    bool testing_draw_bounding_circles;
    bool testing_draw_player_rotation;
} GameState;

#define GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX 5

#define TEXTURE_POS_SPACESHIP_FRIENDLY_BASE     ((Rectangle){320, 0, 96, 96})
#define TEXTURE_POS_SPACESHIP_FRIENDLY_UPGRADED ((Rectangle){304, 384, 96, 96})
#define TEXTURE_POS_SPACESHIP_ENEMY_BASE        ((Rectangle){400, 256, 96, 96})
#define TEXTURE_POS_SPACESHIP_ENEMY_UPGRADED    ((Rectangle){512, 0, 96, 96})

#define TEXTURE_POS_PROJECTILE_PLAYER ((Rectangle){608, 0, 64, 127})
#define TEXTURE_POS_PROJECTILE_ENEMY  ((Rectangle){688, 160, 64, 127})

#define TESTING_PLAYER_ROTATION_DIAGRAM_DISTANCE 300

void GameStateInitialize(void);
void GameStateUpdate(void);  // Call only if the state is initialized
void GameStateCleanup(void);

bool GameStatePlayerAdd(void);
bool GameStatePlayerRemove(void);
bool GameStateIsGameOver();
void GameStateSetGameOver();

Player GameStateGetClosestPlayer(Vector2 position);

void GameStateSetDefaultMappings();

Rectangle SpaceshipTextureLocation(SpaceshipType type);    // Spaceship texture location
Rectangle ProjectileTextureLocation(ProjectileType type);  // Proyectile texture location

f32 ScaleToDelta(f32 value);
Vector2 Vector2ScaleToDelta(Vector2 v);

extern GameState *state;  // Game state global variable

/**
 * Custom for-each-loop to iterate over all the active players.
 *
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index `u8` Index of the current player.
 * @param iteration_var.player `Player *` Pointer to the player struct.
 *
 * Usage:
 * ```
 * ForEachPlayerRef(itr) {
 *     printf("Player number: %d [%p]. Player rotation: %.2f\n", itr.index, itr.player, itr.player->entity.rotation");
 * }
 * ```
 */
#define ForEachPlayerRef(iteration_var) _FOR_EACH_PLAYER_REFERENCE(iteration_var)
/**
 * Custom for-each-loop to iterate over all the active players.
 *
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index `u8` Index of the current player.
 * @param iteration_var.player `Player *` Pointer to the player struct.
 * @param offset Number of players to skip.
 *
 * Usage:
 * ```
 * // Skips the first 2 players
 * ForEachPlayerRefFrom(itr, 2) {
 *     printf("Player number: %d. Player rotation: %.2f\n", itr.index, itr.player.entity.rotation");
 * }
 * ```
 */
#define ForEachPlayerRefFrom(iteration_var, offset) _FOR_EACH_PLAYER_REFERENCE_FROM(iteration_var, offset)

/**
 * Custom for-each-loop to iterate over all the active players.
 *
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index `u8` Index of the current player.
 * @param iteration_var.player `Player` Copy of the player struct.
 *
 * Usage:
 * ```
 * ForEachPlayerVal(itr) {
 *     printf("Player number: %d. Player rotation: %.2f\n", itr.index, itr.player.entity.rotation");
 * }
 * ```
 */
#define ForEachPlayerVal(iteration_var) _FOR_EACH_PLAYER_VALUE(iteration_var)
/**
 * Custom for-each-loop to iterate over all the active players.
 *
 * @param iteration_var Name of the variable where all the iteration information will be stored.
 * @param iteration_var.index `u8` Index of the current player.
 * @param iteration_var.player `Player` Copy of the player struct.
 * @param offset Number of players to skip.
 *
 * Usage:
 * ```
 * // Skips the first 2 players
 * ForEachPlayerValFrom(itr, 2) {
 *     printf("Player number: %d. Player rotation: %.2f\n", itr.index, itr.player.entity.rotation");
 * }
 * ```
 */
#define ForEachPlayerValFrom(iteration_var, offset) _FOR_EACH_PLAYER_VALUE_FROM(iteration_var, offset)

#endif  // __STATE_H__