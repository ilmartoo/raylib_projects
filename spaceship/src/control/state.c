#include "state.h"
#include "entities.h"
#include "raylib.h"

void initialize_game_state(void)
{
    Image spritesheet_image = LoadImage(GET_IMAGE_PATH("spritesheet.png"));

    state = (GameState){
        .time_elapsed = GetTime(),
        .time_delta = GetFrameTime(),

        .players = data_object_pool_create_type(Player),
        .enemies = data_object_pool_create_type(Enemy),
        .projectiles_players = data_object_pool_create_type(Projectile),
        .projectiles_enemies = data_object_pool_create_type(Projectile),

        .spritesheet = LoadTextureFromImage(spritesheet_image),
        .spritesheet_locations_spaceships = {SPACESHIP_FRIENDLY_BASE_TEXTURE_POS, SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS,
                                             SPACESHIP_ENEMY_BASE_TEXTURE_POS, SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS},
        .spritesheet_locations_proyectiles = {PROJECTILE_PLAYER_TEXTURE_POS, PROJECTILE_ENEMY_TEXTURE_POS},

        .fonts = {LoadFont(GET_FONT_PATH("atkinson_hyper_mono_regular.ttf")), LoadFont(GET_FONT_PATH("atkinson_hyper_mono_bold.ttf"))}};

    UnloadImage(spritesheet_image);
}

void clear_game_state(void)
{

    data_object_pool_release(&state.players);
    data_object_pool_release(&state.enemies);
    data_object_pool_release(&state.projectiles_players);
    data_object_pool_release(&state.projectiles_enemies);
    UnloadTexture(state.spritesheet);
    UnloadFont(state.fonts[0]);
    UnloadFont(state.fonts[1]);
}

Rectangle locate_texture_spaceship(SpaceshipType type) { return state.spritesheet_locations_spaceships[type]; }

Rectangle locate_texture_projectile(ProjectileType type) { return state.spritesheet_locations_proyectiles[type]; }

f32 adjust_to_delta(f32 value) { return value * state.time_delta; }

Font font(FontFamily type) { return state.fonts[type]; }
