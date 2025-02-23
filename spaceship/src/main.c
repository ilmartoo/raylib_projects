#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "types.h"
#include "debug.h"
#include "arena.h"
#include "string.h"
#include "entity_pool.h"

//
// Configuration
// :macros
//
#pragma region

#define ASSETS_DIR                 "assets/"
#define IMAGES_DIR                 ASSETS_DIR "images/"
#define AUDIO_DIR                  ASSETS_DIR "audio/"
#define GET_ASSET_PATH(asset_name) (ASSETS_DIR asset_name)
#define GET_IMAGE_PATH(image_name) (IMAGES_DIR image_name)
#define GET_AUDIO_PATH(audio_name) (AUDIO_DIR audio_name)

#define INIT_SCREEN_H 600
#define INIT_SCREEN_W 1200

#pragma endregion

//
// Utils
// :utils
//
#pragma region

f32 vector_distance(Vector2 origin, Vector2 pos)
{
    f32 x = pos.x - origin.x;
    f32 y = pos.y - origin.y;
    f32 x_square = x * x;
    f32 y_square = y * y;
    return sqrt(x_square + y_square);
}

#pragma endregion

//
// Structs
// :structs
//
#pragma region

typedef struct Controls
{
    u32 left;
    u32 right;
    u32 up;
    u32 down;
    u32 shoot;
} Controls;

typedef struct Positional
{
    Vector2 size;
    Vector2 coords;
} Positional;

typedef struct Movable
{
    Vector2 speed;
} Movable;

typedef struct AbilityCooldown
{
    f64 available_at; // The time at which the ability will be available again
    f64 wait_time;    // Cooldown of the ability
} AbilityCooldown;

typedef enum SpaceshipType
{
    SPACESHIP_FRIENDLY_BASE = 0,
#define SPACESHIP_FRIENDLY_BASE_TEXTURE_POS ((Rectangle){320, 0, 96, 96})
    SPACESHIP_FRIENDLY_UPGRADED,
#define SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS ((Rectangle){304, 384, 96, 96})
    SPACESHIP_ENEMY_BASE,
#define SPACESHIP_ENEMY_BASE_TEXTURE_POS ((Rectangle){400, 256, 96, 96})
    SPACESHIP_ENEMY_UPGRADED
#define SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS ((Rectangle){512, 0, 96, 96})
} SpaceshipType;

typedef struct Spaceship
{
    SpaceshipType type;
    Movable movement;
    Positional pos;
} Spaceship;

#define SPACESHIP_MOVEMENT_SPEED_BASE ((Vector2){250, 200})
#define SPACESHIP_SIZE_BASE           ((Vector2){20, 20})

typedef enum BeamType
{
    BEAM_PLAYER = 0,
#define BEAM_PLAYER_TEXTURE_POS ((Rectangle){608, 0, 64, 127})
    BEAM_ENEMY,
#define BEAM_ENEMY_TEXTURE_POS ((Rectangle){688, 160, 64, 127})
} BeamType;

typedef struct Beam
{
    u32 damage;
    f32 range;
    Vector2 origin;
    /* ... */
    BeamType type;
    Movable movement;
    Positional pos;
} Beam;

typedef struct ShootAbility
{
    f32 damage;
    f32 range;
    f32 proyectile_speed;
    Vector2 proyectile_size;
    AbilityCooldown cooldown;
} ShootAbility;

#define SHOOTING_ABILITY_DAMAGE_BASE           10
#define SHOOTING_ABILITY_RANGE_BASE            200
#define SHOOTING_ABILITY_PROYECTILE_SPEED_BASE 450
#define SHOOTING_ABILITY_PROYECTILE_SIZE_BASE  ((Vector2){4, 30})

typedef struct Player
{
    Spaceship spaceship;
    ShootAbility attack;
    /* Player */
    u8 player_number;
    Controls controls;
} Player;

#define PLAYER_ATTACK_SPEED_BASE 0.1f

typedef struct Enemy
{
    Spaceship spaceship;
    ShootAbility attack;
} Enemy;

typedef struct GameState
{
    struct
    {
        f64 elapsed;
        f32 delta;
    } time;
    // Entity pools
    struct
    {
        EntityPool players;
        EntityPool enemies;
        EntityPool player_beams;
        EntityPool enemy_beams;
    } pools;
    struct
    {
        Rectangle spaceships[4];
        Rectangle beams[2];
    } sprite_locations;
    Texture2D spritesheet;
} GameState;

GameState state = {0};

#pragma endregion

//
// Util Functions And Macros
// :utils :functions :macros
//
#pragma region

Rectangle get_spaceship_type_location(SpaceshipType type) { return state.sprite_locations.spaceships[type]; }

Rectangle get_beam_type_location(BeamType type) { return state.sprite_locations.beams[type]; }

f32 adjust_to_delta(f32 weight) { return weight * state.time.delta; }

#pragma endregion

//
// Generator functions
// :generators :functions
//
#pragma region

Spaceship create_spaceship(Vector2 pos, SpaceshipType type)
{
    return (Spaceship){
        .type = type, .movement = {.speed = SPACESHIP_MOVEMENT_SPEED_BASE}, .pos = {.size = SPACESHIP_SIZE_BASE, .coords = pos}};
}

Beam create_beam(BeamType type, f32 damage, f32 range, f32 movement_speed, Vector2 size, Vector2 pos)
{
    return (Beam){.damage = damage,
                  .range = range,
                  .origin = pos,
                  .type = type,
                  .movement = {.speed = {0, movement_speed}},
                  .pos = {.size = size, .coords = pos}};
}

Beam *generate_player_beam(f32 damage, f32 range, f32 movement_speed, Vector2 size, Vector2 pos)
{
    Beam beam = create_beam(BEAM_PLAYER, damage, range, movement_speed, size, pos);
    return entity_pool_add(&state.pools.player_beams, &beam);
}

Beam *generate_enemy_beam(f32 damage, f32 range, f32 movement_speed, Vector2 size, Vector2 pos)
{
    Beam beam = create_beam(BEAM_ENEMY, damage, range, movement_speed, size, pos);
    return entity_pool_add(&state.pools.enemy_beams, &beam);
}

Player *generate_entity_player(Vector2 pos)
{
    Player player = {.spaceship = create_spaceship(pos, SPACESHIP_FRIENDLY_BASE),
                     .attack =
                         {
                             .damage = SHOOTING_ABILITY_DAMAGE_BASE,
                             .range = SHOOTING_ABILITY_RANGE_BASE,
                             .proyectile_speed = SHOOTING_ABILITY_PROYECTILE_SPEED_BASE,
                             .proyectile_size = SHOOTING_ABILITY_PROYECTILE_SIZE_BASE,
                             .cooldown = {.wait_time = PLAYER_ATTACK_SPEED_BASE, .available_at = 0},
                         },
                     .player_number = state.pools.players.chunk_count,
                     .controls = {.left = KEY_LEFT, .right = KEY_RIGHT, .up = KEY_UP, .down = KEY_DOWN, .shoot = KEY_SPACE}};
    return entity_pool_add(&state.pools.players, &player);
}

void init_global_state(void)
{
    Image spritesheet_image = LoadImage(GET_IMAGE_PATH("spritesheet.png"));

    state = (GameState){.time = {.elapsed = GetTime(), .delta = GetFrameTime()},
                        .pools = {.players = entity_pool_create_type(Player),
                                  .enemies = entity_pool_create_type(Enemy),
                                  .player_beams = entity_pool_create_type(Beam),
                                  .enemy_beams = entity_pool_create_type(Beam)},
                        .sprite_locations = {.spaceships = {SPACESHIP_FRIENDLY_BASE_TEXTURE_POS, SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS,
                                                            SPACESHIP_ENEMY_BASE_TEXTURE_POS, SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS},
                                             .beams = {BEAM_PLAYER_TEXTURE_POS, BEAM_ENEMY_TEXTURE_POS}},
                        .spritesheet = LoadTextureFromImage(spritesheet_image)};

    UnloadImage(spritesheet_image);

    generate_entity_player((Vector2){10, 10});
}

void clear_global_state(void)
{
    entity_pool_release(&state.pools.players);
    entity_pool_release(&state.pools.enemies);
    entity_pool_release(&state.pools.player_beams);
    entity_pool_release(&state.pools.enemy_beams);
    UnloadTexture(state.spritesheet);
}

#pragma endregion

//
// Logic Functions
// :logic :functions
//
#pragma region

bool cooldown_up(AbilityCooldown cooldown) { return cooldown.available_at <= state.time.elapsed; }

void cooldown_update(AbilityCooldown *cooldown) { cooldown->available_at = state.time.elapsed + cooldown->wait_time; }

#pragma endregion

//
// Inputs
// :inputs :movement
//
#pragma region

bool beam_entity_move(Beam *b)
{
    if ((float)fabs(vector_distance(b->pos.coords, b->origin)) < b->range)
    {
        b->pos.coords.x -= adjust_to_delta(b->movement.speed.x);
        b->pos.coords.y -= adjust_to_delta(b->movement.speed.y);
        return true;
    }
    return false;
}

void player_entity_move(Player *player)
{
    Spaceship *ship = &player->spaceship;

    bool isLeft = IsKeyDown(player->controls.left);
    bool isRight = IsKeyDown(player->controls.right);
    bool isUp = IsKeyDown(player->controls.up);
    bool isDown = IsKeyDown(player->controls.down);

    if (isLeft != isRight)
    {
        f32 x = adjust_to_delta(ship->movement.speed.x);
        ship->pos.coords.x += isLeft ? -x : x;
    }
    if (isUp != isDown)
    {
        f32 y = adjust_to_delta(ship->movement.speed.y);
        ship->pos.coords.y += isUp ? -y : y;
    }
}

void player_entity_shoot(Player *player)
{
    if (cooldown_up(player->attack.cooldown))
    {
        bool isShooting = IsKeyDown(player->controls.shoot);

        if (isShooting)
        {
            Spaceship ship = player->spaceship;
            Vector2 beam_pos = {ship.pos.coords.x + ((ship.pos.size.x - player->attack.proyectile_size.x) / 2),
                                ship.pos.coords.y - player->attack.proyectile_size.y + (ship.pos.size.y / 2)};
            generate_player_beam(player->attack.damage, player->attack.range, player->attack.proyectile_speed,
                                 player->attack.proyectile_size, beam_pos);
            cooldown_update(&player->attack.cooldown);
        }
    }
}

void update_beam_entities(void)
{
    EntityPool *pool;
    entity_pool_for_each_entity(pool = &state.pools.player_beams, Beam, itr)
    {
        if (!beam_entity_move(itr.entity)) { entity_pool_pop(pool, itr.index); }
    }

    entity_pool_for_each_entity(pool = &state.pools.enemy_beams, Beam, itr)
    {
        if (!beam_entity_move(itr.entity)) { entity_pool_pop(pool, itr.index); }
    }
}

void update_player_entities(void)
{
    entity_pool_for_each_entity(&state.pools.players, Player, itr)
    {
        player_entity_move(itr.entity);
        player_entity_shoot(itr.entity);
    }
}

#pragma endregion

//
// Drawing
// :drawing
//
#pragma region

void draw_spaceship(Spaceship s)
{
    DrawTexturePro(state.spritesheet, get_spaceship_type_location(s.type),
                   (Rectangle){s.pos.coords.x, s.pos.coords.y, s.pos.size.x, s.pos.size.y}, (Vector2){0}, 0, WHITE);
}

void draw_beam(Beam b)
{
    DrawTexturePro(state.spritesheet, get_beam_type_location(b.type),
                   (Rectangle){b.pos.coords.x, b.pos.coords.y, b.pos.size.x, b.pos.size.y}, (Vector2){0}, 0, WHITE);
}

void draw_players(void)
{
    entity_pool_for_each_entity(&state.pools.players, Player, itr) { draw_spaceship(itr.entity->spaceship); }
}

void draw_alive_enemies(void)
{
    entity_pool_for_each_entity(&state.pools.enemies, Enemy, itr) { draw_spaceship(itr.entity->spaceship); }
}

void draw_alive_beams(void)
{
    entity_pool_for_each_entity(&state.pools.player_beams, Beam, itr) { draw_beam(*itr.entity); }
    entity_pool_for_each_entity(&state.pools.enemy_beams, Beam, itr) { draw_beam(*itr.entity); }
}

#pragma endregion

//
// Main function
// :main
//
#pragma region

void setup_window()
{
    const char *game_title = "Spaceship";
    Image icon = LoadImage(GET_IMAGE_PATH("spaceship-icon.png"));

    InitWindow(INIT_SCREEN_W, INIT_SCREEN_H, game_title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(icon);

    UnloadImage(icon);
}

void update_times(void)
{
    state.time.elapsed = GetTime();
    state.time.delta = GetFrameTime();
}

void update_entities(void)
{
    update_player_entities();
    update_beam_entities();
}

void do_drawing(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    draw_players();
    draw_alive_enemies();
    draw_alive_beams();

#ifdef DEBUG
    f32 xo = 5, yo = 5, wo = 100, ho = 100, thick = 2, font_size = 10, margin = 3, x = xo + thick + margin, y = yo + thick + margin;

    DrawRectangleLinesEx((Rectangle){xo, yo, wo, ho}, thick, GREEN);
    DrawRectangle(xo + thick, yo + thick, wo - thick - thick, ho - thick - thick, Fade(WHITE, 0.25));

    DrawText("Debug info", x, y, font_size, WHITE);

    y += font_size + margin;
    DrawText(TextFormat("FPS: %d", GetFPS()), x, y, font_size, WHITE);

    y += font_size + margin;
    DrawText(TextFormat("Enemies [Total]: %lld", entity_pool_chunk_count(state.pools.enemies)), x, y, font_size, WHITE);
    y += font_size;
    DrawText(TextFormat("Enemies [Alive]: %lld", entity_pool_entity_count(state.pools.enemies)), x, y, font_size, WHITE);

    y += font_size + margin;
    DrawText(TextFormat("Beams [Total]: %lld", entity_pool_chunk_count(state.pools.player_beams)), x, y, font_size, WHITE);
    y += font_size;
    DrawText(TextFormat("Beams [Alive]: %lld", entity_pool_entity_count(state.pools.player_beams)), x, y, font_size, WHITE);
#endif
    EndDrawing();
}

int main()
{
    SetTargetFPS(144);

    setup_window();
    init_global_state();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        update_times();
        update_entities();
        // check_collisions();
        do_drawing();
    }

    CloseWindow();
    clear_global_state();

    return 0;
}

#pragma endregion
