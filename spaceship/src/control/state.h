#pragma once
#ifndef __SOURCE_STATE_H_
#define __SOURCE_STATE_H_

#include "entities.h"
#include "object_pool.h"
#include "rayheader.h"
// ----------------------------------------------------------------------------
// ---- Internals -------------------------------------------------------------
// ----------------------------------------------------------------------------

#define __FOR_EACH_PLAYER_REFERENCE_FROM(iteration_var, starting_index)                                                                    \
    for (                                                                                                                                  \
        struct {                                                                                                                           \
            u8 index;                                                                                                                      \
            Player *player;                                                                                                                \
        } iteration_var = {.index = starting_index, .player = &state->players[starting_index]};                                            \
        iteration_var.index < state->player_count;                                                                                         \
        ++iteration_var.index, iteration_var.player = &state->players[iteration_var.index])

#define __FOR_EACH_PLAYER_REFERENCE(iteration_var) __FOR_EACH_PLAYER_REFERENCE_FROM(iteration_var, 0)

#define __FOR_EACH_PLAYER_VALUE_FROM(iteration_var, starting_index)                                                                        \
    for (                                                                                                                                  \
        struct {                                                                                                                           \
            u8 index;                                                                                                                      \
            Player player;                                                                                                                 \
        } iteration_var = {.index = starting_index, .player = state->players[starting_index]};                                             \
        iteration_var.index < state->player_count;                                                                                         \
        ++iteration_var.index, iteration_var.player = state->players[iteration_var.index])

#define __FOR_EACH_PLAYER_VALUE(iteration_var) __FOR_EACH_PLAYER_VALUE_FROM(iteration_var, 0)

// ----------------------------------------------------------------------------
// ---- Game state ------------------------------------------------------------
// ----------------------------------------------------------------------------

#define GAME_STATE_MAX_PLAYERS         4
#define GAME_STATE_TOTAL_INPUT_DEVICES MAX_GAMEPADS + 1 // Keyboard and mouse + max number of gamepads

typedef struct GameState
{
    /* Time */
    f64 time_elapsed;
    f32 time_delta_real;
    f32 time_delta_simulation;
    i16 time_speed_magnitude;
    bool time_running;

    /* Players */
    u8 player_count;
    Player players[GAME_STATE_MAX_PLAYERS];
    bool game_over[GAME_STATE_MAX_PLAYERS];

    /* Inputs */
    Mapping mappings[GAME_STATE_TOTAL_INPUT_DEVICES][ACTION_TYPES_COUNT];
    bool gamepad_locked[MAX_GAMEPADS];

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

void GameStateInitialization(void);
void GameStateUpdate(void); // Call only if the state is initialized
void GameStateCleanup(void);

bool GameStatePlayerAdd(void);
bool GameStatePlayerRemove(void);
bool GameStateIsPlayerGameOver(Player player);
void GameStateSetPlayerGameOver(Player *player);

Player GameStateGetClosestPlayer(Vector2 position);

void GameStateSetDefaultMappings(InputDevice device);

Rectangle SpaceshipTextureLocation(SpaceshipType type);   // Spaceship texture location
Rectangle ProjectileTextureLocation(ProjectileType type); // Proyectile texture location

f32 ScaleToDelta(f32 value);
Vector2 Vector2ScaleToDelta(Vector2 v);

extern GameState *state; // Game state global variable

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
#define ForEachPlayerRef(iteration_var) __FOR_EACH_PLAYER_REFERENCE(iteration_var)
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
#define ForEachPlayerRefFrom(iteration_var, offset) __FOR_EACH_PLAYER_REFERENCE_FROM(iteration_var, offset)

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
#define ForEachPlayerVal(iteration_var) __FOR_EACH_PLAYER_VALUE(iteration_var)
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
#define ForEachPlayerValFrom(iteration_var, offset) __FOR_EACH_PLAYER_VALUE_FROM(iteration_var, offset)

#endif // __SOURCE_STATE_H_