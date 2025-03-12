#include "state.h"
#include "entities.h"
#include "files.h"
#include "rayheader.h"

GameState *state = NULL;

void GameStateInitialization(void)
{
    if (state == NULL) { GameState *state = reserve(GameState); }

    Image spritesheet_image = LoadImage(path_image("spritesheet.png"));

    *state =
        (GameState){.time_elapsed = 0,
                    .time_delta_real = 0,
                    .time_speed_magnitude = 0,
                    .time_delta_simulation = 0,
                    .time_running = true,

                    .input_profiles = ObjectPoolCreateType(PlayerControls),

                    .players = ObjectPoolCreateType(Player),
                    .enemies = ObjectPoolCreateType(Enemy),
                    .projectiles_players = ObjectPoolCreateType(Projectile),
                    .projectiles_enemies = ObjectPoolCreateType(Projectile),

                    .spritesheet = LoadTextureFromImage(spritesheet_image),
                    .spritesheet_locations_spaceships = {TEXTURE_POS_SPACESHIP_FRIENDLY_BASE,
                                                         TEXTURE_POS_SPACESHIP_FRIENDLY_UPGRADED,
                                                         TEXTURE_POS_SPACESHIP_ENEMY_BASE,
                                                         TEXTURE_POS_SPACESHIP_ENEMY_UPGRADED},
                    .spritesheet_locations_proyectiles = {TEXTURE_POS_PROJECTILE_PLAYER, TEXTURE_POS_PROJECTILE_ENEMY},

                    .fonts = {LoadFont(path_font("atkinson_hyper_mono_regular.ttf")), LoadFont(path_font("atkinson_hyper_mono_bold.ttf"))}};

    UnloadImage(spritesheet_image);
}

void GameStateUpdate(void)
{
    f32 delta = GetFrameTime();

    state->time_elapsed += delta;
    state->time_delta_real = delta;

    state->time_delta_simulation = state->time_speed_magnitude == 0
                                       ? state->time_delta_real
                                       : state->time_delta_real + (state->time_delta_real * state->time_speed_magnitude * 0.2f);
}

void GameStateCleanup(void)
{
    if (state != NULL)
    {
        ObjectPoolDelete(&state->players);
        ObjectPoolDelete(&state->enemies);
        ObjectPoolDelete(&state->projectiles_players);
        ObjectPoolDelete(&state->projectiles_enemies);
        UnloadTexture(state->spritesheet);
        UnloadFont(state->fonts[0]);
        UnloadFont(state->fonts[1]);
        free(state);
        state = NULL;
    }
}

Rectangle SpaceshipTextureLocation(SpaceshipType type) { return state->spritesheet_locations_spaceships[type]; }

Rectangle ProjectileTextureLocation(ProjectileType type) { return state->spritesheet_locations_proyectiles[type]; }

f32 ScaleToDelta(f32 value) { return value * state->time_delta_simulation; }

Vector2 Vector2ScaleToDelta(Vector2 v) { return Vector2Scale(v, state->time_delta_simulation); }

Font FontFromFamily(FontFamily type) { return state->fonts[type]; }
