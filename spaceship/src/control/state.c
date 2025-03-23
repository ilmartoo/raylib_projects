#include <stdlib.h>

#include "debug.h"
#include "entities.h"
#include "files.h"
#include "memory_utils.h"
#include "rayheader.h"
#include "state.h"

GameState *state = NULL;

void GameStateInitialization(void)
{
    if (state == NULL) { state = reserve(GameState); }

    Image spritesheet_image = LoadImage(path_image("spritesheet.png"));

    *state = (GameState){
        .time_elapsed = 0,
        .time_delta_real = 0,
        .time_speed_magnitude = 0,
        .time_delta_simulation = 0,
        .time_running = true,

        .player_count = 0, // No players for now
        .players = {0},    // All non-initialized
        .game_over = {0},  // All false

        .mappings = {0},       // All non-initialized
        .gamepad_locked = {0}, // All gamepads free to use

        .enemies = ObjectPoolCreateType(Enemy),
        .projectiles_players = ObjectPoolCreateType(Projectile),
        .projectiles_enemies = ObjectPoolCreateType(Projectile),

        .spritesheet = LoadTextureFromImage(spritesheet_image),
        .spritesheet_locations_spaceships = {TEXTURE_POS_SPACESHIP_FRIENDLY_BASE,
                                             TEXTURE_POS_SPACESHIP_FRIENDLY_UPGRADED,
                                             TEXTURE_POS_SPACESHIP_ENEMY_BASE,
                                             TEXTURE_POS_SPACESHIP_ENEMY_UPGRADED},
        .spritesheet_locations_proyectiles = {TEXTURE_POS_PROJECTILE_PLAYER, TEXTURE_POS_PROJECTILE_ENEMY},

        .font = LoadFont(path_font("Kenney Future.ttf")),

#ifdef DEBUG
        .testing_draw_bounding_circles = true,
        .testing_draw_player_rotation = true,
#else
        .testing_draw_bounding_circles = false,
        .testing_draw_player_rotation = false,
#endif
    };

    UnloadImage(spritesheet_image);

    // Set mappings
    for (InputDevice device = 0; device <= INPUT_DEVICE_MOUSE_AND_KEYBOARD; ++device) { GameStateSetDefaultMappings(device); }
}

void GameStateUpdate(void)
{
    f32 delta = GetFrameTime();

    state->time_delta_real = delta;
    state->time_delta_simulation = state->time_speed_magnitude == 0
                                       ? state->time_delta_real
                                       : state->time_delta_real + (state->time_delta_real * state->time_speed_magnitude * 0.2f);
    state->time_elapsed += state->time_delta_simulation;
}

void GameStateCleanup(void)
{
    if (state != NULL)
    {
        ObjectPoolDelete(&state->enemies);
        ObjectPoolDelete(&state->projectiles_players);
        ObjectPoolDelete(&state->projectiles_enemies);
        UnloadTexture(state->spritesheet);
        UnloadFont(state->font);
        free(state);
        state = NULL;
    }
}

bool GameStatePlayerAdd(void)
{
    u8 count = state->player_count;
    if (count < GAME_STATE_MAX_PLAYERS)
    {
        // Prioritize gamepads, but set mouse and keyboard as default fallback
        InputDevice device = INPUT_DEVICE_MOUSE_AND_KEYBOARD;
        for (u8 i = 0; i < count; ++i)
        {
            if (IsGamepadAvailable(i) && !state->gamepad_locked[i])
            {
                state->gamepad_locked[i] = true;
                device = i;
            }
        }

        state->players[count] =
            PlayerCreate(device, count, SPACESHIP_FRIENDLY_BASE, Vector2Scale(Vector2From(GetScreenWidth(), GetScreenHeight()), 0.5f));
        state->game_over[count] = false;

        ++state->player_count;
        return true;
    }
    return false;
}

bool GameStatePlayerRemove(void)
{
    u8 count = state->player_count;
    if (count > 0)
    {
        state->gamepad_locked[state->players[count]._device] = false;

        --state->player_count;
        return true;
    }
    return false;
}

bool GameStateIsPlayerGameOver(Player player) { return state->game_over[player._player_index]; }

void GameStateSetPlayerGameOver(Player *player) { state->game_over[player->_player_index] = true; }

Player GameStateGetClosestPlayer(Vector2 position)
{
    u8 closest = 0;
    f32 closest_distance = Vector2Distance(state->players[0].entity.position, position);
    ForEachPlayerValFrom(iter, 1)
    {
        f32 distance = Vector2Distance(iter.player.entity.position, position);
        if (closest_distance > distance)
        {
            closest = iter.index;
            closest_distance = distance;
        }
    }
    return state->players[closest];
}

#define __GameStateSetDefaultMappings(device, default_values)                                                                              \
    do {                                                                                                                                   \
        Mapping default_mappings[ACTION_TYPES_COUNT] = default_values;                                                                     \
        memory_copy(state->mappings[device], default_mappings, sizeof(Mapping) * ACTION_TYPES_COUNT);                                      \
    } while (0)

void GameStateSetDefaultMappings(InputDevice device)
{
    if (device == INPUT_DEVICE_MOUSE_AND_KEYBOARD) { __GameStateSetDefaultMappings(device, ACTION_MAPPING_DEFAULT_MOUSE_AND_KEYBOARD); }
    else { __GameStateSetDefaultMappings(device, ACTION_MAPPING_DEFAULT_GAMEPAD); }
}

Rectangle SpaceshipTextureLocation(SpaceshipType type) { return state->spritesheet_locations_spaceships[type]; }

Rectangle ProjectileTextureLocation(ProjectileType type) { return state->spritesheet_locations_proyectiles[type]; }

f32 ScaleToDelta(f32 value) { return value * state->time_delta_simulation; }

Vector2 Vector2ScaleToDelta(Vector2 v) { return Vector2Scale(v, state->time_delta_simulation); }
