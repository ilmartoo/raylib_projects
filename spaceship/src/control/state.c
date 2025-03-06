#include "state.h"
#include "entities.h"
#include "files.h"
#include "raylib.h"
#include "raymath.h"

GameState state = {0};

void game_state_initialize(void)
{
    Image spritesheet_image = LoadImage(path_image("spritesheet.png"));

    state =
        (GameState){.time_elapsed = 0,
                    .time_delta_real = 0,
                    .time_speed_magnitude = 0,
                    .time_delta_simulation = 0,
                    .time_running = true,

                    .players = data_object_pool_create_type(Player),
                    .enemies = data_object_pool_create_type(Enemy),
                    .projectiles_players = data_object_pool_create_type(Projectile),
                    .projectiles_enemies = data_object_pool_create_type(Projectile),

                    .spritesheet = LoadTextureFromImage(spritesheet_image),
                    .spritesheet_locations_spaceships = {SPACESHIP_FRIENDLY_BASE_TEXTURE_POS, SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS,
                                                         SPACESHIP_ENEMY_BASE_TEXTURE_POS, SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS},
                    .spritesheet_locations_proyectiles = {PROJECTILE_PLAYER_TEXTURE_POS, PROJECTILE_ENEMY_TEXTURE_POS},

                    .fonts = {LoadFont(path_font("atkinson_hyper_mono_regular.ttf")), LoadFont(path_font("atkinson_hyper_mono_bold.ttf"))}};

    UnloadImage(spritesheet_image);
}

void game_state_update(void)
{
    f32 delta = GetFrameTime();

    state.time_elapsed += delta;
    state.time_delta_real = delta;

    state.time_delta_simulation = state.time_speed_magnitude == 0
                                      ? state.time_delta_real
                                      : state.time_delta_real + (state.time_delta_real * state.time_speed_magnitude * 0.2);
}

void game_state_clear(void)
{

    data_object_pool_delete(&state.players);
    data_object_pool_delete(&state.enemies);
    data_object_pool_delete(&state.projectiles_players);
    data_object_pool_delete(&state.projectiles_enemies);
    UnloadTexture(state.spritesheet);
    UnloadFont(state.fonts[0]);
    UnloadFont(state.fonts[1]);
}

Rectangle locate_texture_spaceship(SpaceshipType type) { return state.spritesheet_locations_spaceships[type]; }

Rectangle locate_texture_projectile(ProjectileType type) { return state.spritesheet_locations_proyectiles[type]; }

f32 adjust_to_time_delta(f32 value) { return value * state.time_delta_simulation; }

Vector2 adjust_vector2_to_time_delta(Vector2 v) { return Vector2Scale(v, state.time_delta_simulation); }

Font font(FontFamily type) { return state.fonts[type]; }
