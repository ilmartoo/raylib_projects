#include <math.h>
#include "raylib.h"
#include "types.h"
#include "helpers.h"
#include "debug.h"
#include "arena.h"

//
// Configuration
// :state
//
#pragma region

#define ASSETS_DIR "assets/"
#define GET_ASSET_PATH(asset_name) ASSETS_DIR asset_name

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

typedef struct Spaceship {
    // Characteristics
    f32 attack_speed;
    Vector2 movement_speed;
    f32 attack_cooldown;
    // Beam shooting
    f32 beam_speed;
    f32 beam_damage;
    f32 beam_range;
    // World representation
    Vector2 size;
    Vector2 pos;
} Spaceship;

typedef struct Player {
    Spaceship spaceship;
    i32 ally_spaceship_type;
} Player;

typedef struct Enemy {
    Spaceship spaceship;
    i32 enemy_spaceship_type;
} Enemy;

typedef struct Beam {
    // Characteristics
    bool is_ally;
    u32 damage;
    f32 movement_speed;
    // Deseasing
    Vector2 origin;
    f32 range;
    bool alive;
    // World representation
    Vector2 size;
    Vector2 pos;
} Beam;

typedef struct EntityPool {
    Arena arena;
    usize total_entities;
} EntityPool;

typedef struct GameState {
    f32 delta_seconds;
    struct {
        Player player;
        EntityPool enemies;
        EntityPool beams;
    } entities;
    struct {
        Rectangle ally_spaceship;
        Rectangle ally_beam;
        Rectangle enemy_spaceship;
        Rectangle enemy_beam;
    } sprite_locations;
    Texture2D spritesheet;
} GameState;

#pragma endregion

//
// Generator functions
// :generation :functions
//
#pragma region

Spaceship create_base_spaceship(Vector2 pos)
{
    return (Spaceship) {
        .attack_speed = 1.5f,
        .movement_speed = { 250, 200 },
        .attack_cooldown = 0,

        .beam_speed = 400,
        .beam_damage = 10,
        .beam_range = 40,

        .pos = pos,
        .size = { 20, 20 }
    };
}

Beam create_beam(bool is_ally, f32 speed, f32 damage, f32 range, Vector2 pos)
{
    return (Beam) {
        .is_ally = is_ally,
        .damage = damage,
        .movement_speed = 350,

        .origin = pos,
        .range = range,
        .alive = true,

        .size = (Vector2) { 5, 10 },
        .pos = pos
    };
}

void player_shoot_beam()
{
}

#pragma endregion

//
// Game state
// :game_config
//
#pragma region

GameState* state;

void init_global_state(void)
{
    Image spritesheet_image = LoadImage(GET_ASSET_PATH("spritesheet.png"));

    state = new (GameState);
    *state = (GameState) {
        .delta_seconds = 0,
        .entities = {
            .player = create_base_spaceship((Vector2) { 10, 10 }),
            .enemies = create_entity_pool(),
            .beams = create_entity_pool() },
        .sprite_locations = { .ally_spaceship = { 320, 0, 96, 96 }, .ally_beam = { 688, 160, 64, 127 }, .enemy_spaceship = { 400, 256, 96, 96 }, .enemy_beam = { 608, 0, 64, 127 } },
        .spritesheet = LoadTextureFromImage(spritesheet_image)
    };

    UnloadImage(spritesheet_image);
}

void free_global_state(void)
{
    ArenaRelease(&state->entities.enemies);
    UnloadTexture(state->spritesheet);
    free(state);
}

#pragma endregion

//
// Movement
// :movement
//
#pragma region

void move_beam(Beam* b)
{
    if (b->alive) {
        if ((float)fabs(vector_distance(b->pos, b->origin)) < b->range) {
            b->pos.y -= b->movement_speed * state->delta_seconds;
        } else {
            b->alive = false;
        }
    }
}

void move_player(void)
{
    Spaceship* p_ship = &state->entities.player.spaceship;

    bool isLeft = IsKeyDown(KEY_LEFT);
    bool isRight = IsKeyDown(KEY_RIGHT);
    bool isUp = IsKeyDown(KEY_UP);
    bool isDown = IsKeyDown(KEY_DOWN);

    if (isLeft != isRight) {
        f32 x = p_ship->movement_speed.x * state->delta_seconds;
        p_ship->pos.x += isLeft ? -x : x;
    }
    if (isUp != isDown) {
        f32 y = p_ship->movement_speed.y * state->delta_seconds;
        p_ship->pos.y += isUp ? -y : y;
    }
}

void do_movement(void)
{
    move_player();
}

#pragma endregion

//
// Drawing
// :drawing
//
#pragma region

void draw_spaceship(Spaceship s)
{
    DrawTexturePro(
        state->spritesheet,
        state->sprite_locations.ally_spaceship,
        (Rectangle) {
            s.pos.x,
            s.pos.y,
            s.size.x,
            s.size.y },
        (Vector2) { 0 },
        0,
        WHITE);
}

void draw_beam(Beam b)
{
    DrawTexturePro(
        state->spritesheet,
        b.is_ally ? state->sprite_locations.ally_beam : state->sprite_locations.enemy_beam,
        (Rectangle) {
            b.pos.x,
            b.pos.y,
            b.size.x,
            b.size.y },
        (Vector2) { 0 },
        0,
        WHITE);
}

void draw_player(void)
{
    draw_spaceship(state->entities.player.spaceship);
}

void draw_alive_beams(void)
{
    EntityPool pool = state->entities.beams;

    for (i32 idx = 0; idx < pool.total_entities; ++idx) {
        Beam beam = ((Beam*)pool.arena.stack)[idx];
        if (beam.alive) {
            draw_beam(beam);
        }
    }
}

void draw_alive_beams(void)
{
    EntityPool pool = state->entities.beams;

    for (i32 idx = 0; idx < pool.total_entities; ++idx) {
        Beam beam = ((Beam*)pool.arena.stack)[idx];
        if (beam.alive) {
            draw_beam(beam);
        }
    }
}

void do_drawing(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    draw_player();

    DEBUG_BLOCK({
        DrawFPS(0, 0);
    });

    EndDrawing();
}

#pragma endregion

//
// Main function
// :main
//
#pragma region

void setup_window()
{
    string gameTitle = "Spaceship";
    Image icon = LoadImage(GET_ASSET_PATH("spaceship-icon.png"));

    InitWindow(INIT_SCREEN_W, INIT_SCREEN_H, gameTitle);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(icon);

    UnloadImage(icon);
}

int main()
{
    SetTargetFPS(60);

    setup_window();
    init_global_state();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        state->delta_seconds = GetFrameTime();

        // check_collisions();
        do_movement();
        do_drawing();
    }

    CloseWindow();

    return 0;
}

#pragma endregion
