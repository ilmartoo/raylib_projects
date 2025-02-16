#include <stdio.h>
#include <math.h>
#include "raylib.h"
#include "types.h"
#include "helpers.h"
#include "debug.h"
#include "arena.h"

//
// Configuration
// :macros
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

typedef struct Controls {
    u32 left;
    u32 right;
    u32 up;
    u32 down;
    u32 shoot;
} Controls;

typedef enum AllySpaceshipType {
    ALLY_BASE_SPACESHIP = 0,
#define ALLY_BASE_SPACESHIP_TEXTURE_POS ((Rectangle) { 320, 0, 96, 96 })
    ALLY_UPGRADED_SPACESHIP = 1
#define ALLY_UPGRADED_SPACESHIP_TEXTURE_POS ((Rectangle) { 304, 384, 96, 96 })
} AllySpaceshipType;

typedef enum EnemySpaceshipType {
    ENEMY_BASE_SPACESHIP = 0,
#define ENEMY_BASE_SPACESHIP_TEXTURE_POS ((Rectangle) { 400, 256, 96, 96 })
    ENEMY_UPGRADED_SPACESHIP = 1
#define ENEMY_UPGRADED_SPACESHIP_TEXTURE_POS ((Rectangle) { 512, 0, 96, 96 })
} EnemySpaceshipType;

typedef struct Spaceship {
    // Characteristics
    f32 attack_speed;
    Vector2 movement_speed;
    // Beam shooting
    f32 beam_speed;
    f32 beam_damage;
    f32 beam_range;
    Vector2 beam_size;
    // World representation
    Vector2 size;
    Vector2 pos;
} Spaceship;

#define SPACESHIP_ATTACK_SPEED_BASE 0.75f
#define SPACESHIP_MOVEMENT_SPEED_BASE ((Vector2) { 250, 200 })
#define SPACESHIP_SIZE_BASE ((Vector2) { 20, 20 })

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

#define BEAM_SPEED_BASE 450
#define BEAM_DAMAGE_BASE 10
#define BEAM_RANGE_BASE 200
#define BEAM_SIZE_BASE ((Vector2) { 6, 30 })

#define ALLY_BEAM_TEXTURE_POS ((Rectangle) { 608, 0, 64, 127 })
#define ENEMY_BEAM_TEXTURE_POS ((Rectangle) { 688, 160, 64, 127 })

typedef struct Player {
    // Spaceship
    Spaceship spaceship;
    AllySpaceshipType spaceship_type;
    // Cooldowns
    f32 attack_cooldown;
    // Player
    u8 player_number;
    Controls controls;
} Player;

typedef struct Enemy {
    // Spaceship
    Spaceship spaceship;
    EnemySpaceshipType spaceship_type;
    // Cooldowns
    f32 attack_cooldown;
    // Alive
    bool alive;
} Enemy;

typedef struct EntityPool {
    Arena arena;
    usize length;
} EntityPool;

typedef struct GameState {
    f32 delta_seconds;
    struct {
        EntityPool players;
        EntityPool enemies;
        EntityPool beams;
    } entities;
    struct {
        Rectangle ally_spaceships[2];
        Rectangle enemy_spaceships[2];
        Rectangle ally_beam;
        Rectangle enemy_beam;
    } sprite_locations;
    Texture2D spritesheet;
} GameState;

GameState* state;

#pragma endregion

//
// Generator functions
// :generators :functions
//
#pragma region

EntityPool create_entity_pool(void)
{
    return (EntityPool) {
        .arena = ArenaCreate(),
        .length = 0
    };
}

void free_entity_pool(EntityPool* pool)
{
    ArenaClear(&pool->arena);
}

Spaceship create_spaceship(Vector2 pos)
{
    return (Spaceship) {
        .attack_speed = SPACESHIP_ATTACK_SPEED_BASE,
        .movement_speed = SPACESHIP_MOVEMENT_SPEED_BASE,

        .beam_speed = BEAM_SPEED_BASE,
        .beam_damage = BEAM_DAMAGE_BASE,
        .beam_range = BEAM_RANGE_BASE,
        .beam_size = BEAM_SIZE_BASE,

        .pos = pos,
        .size = SPACESHIP_SIZE_BASE
    };
}

Beam create_beam(bool is_ally, f32 movement_speed, f32 damage, f32 range, Vector2 size, Vector2 pos)
{
    return (Beam) {
        .is_ally = is_ally,
        .damage = damage,
        .movement_speed = movement_speed,

        .origin = pos,
        .range = range,
        .alive = true,

        .size = size,
        .pos = pos
    };
}

Beam* generate_entity_beam(bool is_ally, f32 movement_speed, f32 damage, f32 range, Vector2 size, Vector2 pos)
{
    Beam* beam = ArenaPushType(&state->entities.beams.arena, Beam);
    *beam = create_beam(is_ally, movement_speed, damage, range, size, pos);
    ++state->entities.beams.length;
    return beam;
}

Player* generate_entity_player(Vector2 pos)
{
    Player* player = ArenaPushType(&state->entities.players.arena, Player);
    *player = (Player) {
        .spaceship = create_spaceship(pos),
        .spaceship_type = ALLY_BASE_SPACESHIP,

        .attack_cooldown = 0,

        .player_number = state->entities.players.length,
        .controls = {
            .left = KEY_LEFT,
            .right = KEY_RIGHT,
            .up = KEY_UP,
            .down = KEY_DOWN,
            .shoot = KEY_SPACE }
    };
    ++state->entities.players.length;
    return player;
}

void init_global_state(void)
{
    Image spritesheet_image = LoadImage(GET_ASSET_PATH("spritesheet.png"));

    state = new (GameState);
    *state = (GameState) {
        .delta_seconds = 0,
        .entities = {
            .players = create_entity_pool(),
            .enemies = create_entity_pool(),
            .beams = create_entity_pool() },
        .sprite_locations = { .ally_spaceships = { ALLY_BASE_SPACESHIP_TEXTURE_POS, ALLY_UPGRADED_SPACESHIP_TEXTURE_POS }, .enemy_spaceships = { ENEMY_BASE_SPACESHIP_TEXTURE_POS, ENEMY_UPGRADED_SPACESHIP_TEXTURE_POS }, .ally_beam = ALLY_BEAM_TEXTURE_POS, .enemy_beam = ENEMY_BEAM_TEXTURE_POS },
        .spritesheet = LoadTextureFromImage(spritesheet_image)
    };

    UnloadImage(spritesheet_image);

    generate_entity_player((Vector2) { 10, 10 });
}

void free_global_state(void)
{
    free_entity_pool(&state->entities.players);
    free_entity_pool(&state->entities.enemies);
    free_entity_pool(&state->entities.beams);
    UnloadTexture(state->spritesheet);
    free(state);
}

#pragma endregion

//
// Logic Functions
// :logic :functions
//
#pragma region

Player* get_player(u32 index)
{
    return &((Player*)state->entities.players.arena.stack)[index];
}

Enemy* get_enemy(u32 index)
{
    return &((Enemy*)state->entities.enemies.arena.stack)[index];
}

Beam* get_beam(u32 index)
{
    return &((Beam*)state->entities.beams.arena.stack)[index];
}

#pragma endregion

//
// Inputs
// :inputs :movement
//
#pragma region

void beam_entity_move(Beam* b)
{
    if (b->alive) {
        if ((float)fabs(vector_distance(b->pos, b->origin)) < b->range) {
            b->pos.y -= b->movement_speed * state->delta_seconds;
        } else {
            b->alive = false;
        }
    }
}

void player_entity_move(Player* player)
{
    Spaceship* ship = &player->spaceship;

    bool isLeft = IsKeyDown(player->controls.left);
    bool isRight = IsKeyDown(player->controls.right);
    bool isUp = IsKeyDown(player->controls.up);
    bool isDown = IsKeyDown(player->controls.down);

    if (isLeft != isRight) {
        f32 x = ship->movement_speed.x * state->delta_seconds;
        ship->pos.x += isLeft ? -x : x;
    }
    if (isUp != isDown) {
        f32 y = ship->movement_speed.y * state->delta_seconds;
        ship->pos.y += isUp ? -y : y;
    }
}

void player_entity_shoot(Player* player)
{
    if (player->attack_cooldown <= 0) {
        bool isShooting = IsKeyDown(player->controls.shoot);

        if (isShooting) {
            Spaceship ship = player->spaceship;
            Vector2 beam_pos = {
                ship.pos.x + ((ship.size.x - ship.beam_size.x) / 2),
                ship.pos.y - ship.beam_size.y + (ship.size.y / 2)
            };
            generate_entity_beam(true, ship.beam_speed, ship.beam_damage, ship.beam_range, ship.beam_size, beam_pos);
            player->attack_cooldown = player->spaceship.attack_speed;
        }
    }
}

void update_beam_entities(void)
{
    for (u32 idx = 0; idx < state->entities.beams.length; ++idx) {
        beam_entity_move(get_beam(idx));
    }
}

void update_player_entities(void)
{
    for (u32 idx = 0; idx < state->entities.players.length; ++idx) {
        Player* player = get_player(idx);
        player_entity_move(player);
        player_entity_shoot(player);
    }
}

#pragma endregion

//
// Drawing
// :drawing
//
#pragma region

void draw_spaceship(Spaceship s, Rectangle texture_pos)
{
    DrawTexturePro(
        state->spritesheet,
        texture_pos,
        (Rectangle) {
            s.pos.x,
            s.pos.y,
            s.size.x,
            s.size.y },
        (Vector2) { 0 },
        0,
        WHITE);
}

void draw_beam(Beam b, Rectangle texture_pos)
{
    DrawTexturePro(
        state->spritesheet,
        texture_pos,
        (Rectangle) {
            b.pos.x,
            b.pos.y,
            b.size.x,
            b.size.y },
        (Vector2) { 0 },
        0,
        WHITE);
}

void draw_players(void)
{
    for (u32 idx = 0; idx < state->entities.players.length; ++idx) {
        Player player = *get_player(idx);
        draw_spaceship(player.spaceship, state->sprite_locations.ally_spaceships[player.spaceship_type]);
    }
}

void draw_alive_enemies(void)
{
    for (u32 idx = 0; idx < state->entities.enemies.length; ++idx) {
        Enemy enemy = *get_enemy(idx);
        if (enemy.alive) {
            draw_spaceship(enemy.spaceship, state->sprite_locations.ally_spaceships[enemy.spaceship_type]);
        }
    }
}

void draw_alive_beams(void)
{
    for (u32 idx = 0; idx < state->entities.beams.length; ++idx) {
        Beam beam = *get_beam(idx);
        if (beam.alive) {
            draw_beam(beam, beam.is_ally ? state->sprite_locations.ally_beam : state->sprite_locations.enemy_beam);
        }
    }
}

#pragma endregion

//
// Cooldowns
// :cooldown
//
#pragma region

void reduce_cooldowns_players(void)
{
    for (u32 idx = 0; idx < state->entities.players.length; ++idx) {
        Player* player = get_player(idx);
        if (player->attack_cooldown > 0) {
            player->attack_cooldown -= state->delta_seconds;
        }
    }
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

void update_delta(void)
{
    state->delta_seconds = GetFrameTime();
}

void reduce_cooldowns(void)
{
    reduce_cooldowns_players();
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

    DEBUG_BLOCK({
        DrawFPS(0, 0);
        char delta_time[20];
        sprintf(delta_time, "%.6f", state->delta_seconds);
        DrawText(delta_time, 0, 20, 16, WHITE);
    });
    EndDrawing();
}

int main()
{
    SetTargetFPS(60);

    setup_window();
    init_global_state();

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        update_delta();
        reduce_cooldowns();
        update_entities();
        // check_collisions();
        do_drawing();
    }

    CloseWindow();
    free_global_state();

    return 0;
}

#pragma endregion
