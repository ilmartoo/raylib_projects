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
#define IMAGES_DIR ASSETS_DIR "images/"
#define AUDIO_DIR ASSETS_DIR "audio/"
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

typedef struct Controls {
    u32 left;
    u32 right;
    u32 up;
    u32 down;
    u32 shoot;
} Controls;

typedef enum SpaceshipType {
    SPACESHIP_FRIENDLY_BASE = 0,
#define SPACESHIP_FRIENDLY_BASE_TEXTURE_POS ((Rectangle) { 320, 0, 96, 96 })
    SPACESHIP_FRIENDLY_UPGRADED,
#define SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS ((Rectangle) { 304, 384, 96, 96 })
    SPACESHIP_ENEMY_BASE,
#define SPACESHIP_ENEMY_BASE_TEXTURE_POS ((Rectangle) { 400, 256, 96, 96 })
    SPACESHIP_ENEMY_UPGRADED
#define SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS ((Rectangle) { 512, 0, 96, 96 })
} SpaceshipType;

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

#define SPACESHIP_ATTACK_SPEED_BASE 0.5f
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
#define BEAM_SIZE_BASE ((Vector2) { 4, 30 })

#define FRIENDLY_BEAM_TEXTURE_POS ((Rectangle) { 608, 0, 64, 127 })
#define ENEMY_BEAM_TEXTURE_POS ((Rectangle) { 688, 160, 64, 127 })

typedef struct Player {
    // Spaceship
    Spaceship spaceship;
    SpaceshipType spaceship_type;
    // Cooldowns
    f32 attack_cooldown;
    // Player
    u8 player_number;
    Controls controls;
} Player;

typedef struct Enemy {
    // Spaceship
    Spaceship spaceship;
    SpaceshipType spaceship_type;
    // Cooldowns
    f32 attack_cooldown;
    // Alive
    bool alive;
} Enemy;

typedef struct EntityPool {
    Arena arena;
    usize length;
#ifdef DEBUG
    usize alive_count;
#endif
} EntityPool;

typedef struct GameState {
    f32 delta_seconds;
    struct {
        EntityPool players;
        EntityPool enemies;
        EntityPool beams;
    } entities;
    struct {
        Rectangle spaceships[4];
        Rectangle beam_friendly;
        Rectangle beam_enemy;
    } sprite_locations;
    Texture2D spritesheet;
} GameState;

GameState* state;

#pragma endregion

//
// Util Functions
// :utils :functions
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
// Generator functions
// :generators :functions
//
#pragma region

EntityPool create_entity_pool(void)
{
    return (EntityPool) {
#ifdef DEBUG
        .alive_count = 0,
#endif
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
    Beam new_beam = create_beam(is_ally, movement_speed, damage, range, size, pos);
    Beam* beam = NULL;

    EntityPool* pool = &state->entities.beams;
    for (u32 idx = 0; idx < pool->length; ++idx) {
        beam = get_beam(idx);
        if (!beam->alive) {
            *beam = new_beam;
            break;
        }
        beam = NULL;
    }

    if (beam == NULL) {
        beam = ArenaPushType(&state->entities.beams.arena, Beam);
        *beam = new_beam;
        ++state->entities.beams.length;
    }

#ifdef DEBUG
    ++state->entities.beams.alive_count;
#endif
    return beam;
}

Player* generate_entity_player(Vector2 pos)
{
    Player* player = ArenaPushType(&state->entities.players.arena, Player);
    *player = (Player) {
        .spaceship = create_spaceship(pos),
        .spaceship_type = SPACESHIP_FRIENDLY_BASE,

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
    Image spritesheet_image = LoadImage(GET_IMAGE_PATH("spritesheet.png"));

    state = new (GameState);
    *state = (GameState) {
        .delta_seconds = 0,
        .entities = {
            .players = create_entity_pool(),
            .enemies = create_entity_pool(),
            .beams = create_entity_pool() },
        .sprite_locations = { .spaceships = { SPACESHIP_FRIENDLY_BASE_TEXTURE_POS, SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS, SPACESHIP_ENEMY_BASE_TEXTURE_POS, SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS }, .beam_friendly = FRIENDLY_BEAM_TEXTURE_POS, .beam_enemy = ENEMY_BEAM_TEXTURE_POS },
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

#pragma endregion

//
// Inputs
// :inputs :movement
//
#pragma region

bool beam_entity_move(Beam* b)
{
    if ((float)fabs(vector_distance(b->pos, b->origin)) < b->range) {
        b->pos.y -= b->movement_speed * state->delta_seconds;
        return true;
    }
    return false;
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
        Beam* beam = get_beam(idx);
        if (beam->alive) {
            beam->alive = beam_entity_move(beam);
#ifdef DEBUG
            if (!beam->alive) {
                --state->entities.beams.alive_count;
            }
#endif
        }
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
        draw_spaceship(player.spaceship, state->sprite_locations.spaceships[player.spaceship_type]);
    }
}

void draw_alive_enemies(void)
{
    for (u32 idx = 0; idx < state->entities.enemies.length; ++idx) {
        Enemy enemy = *get_enemy(idx);
        if (enemy.alive) {
            draw_spaceship(enemy.spaceship, state->sprite_locations.spaceships[enemy.spaceship_type]);
        }
    }
}

void draw_alive_beams(void)
{
    for (u32 idx = 0; idx < state->entities.beams.length; ++idx) {
        Beam beam = *get_beam(idx);
        if (beam.alive) {
            draw_beam(beam, beam.is_ally ? state->sprite_locations.beam_friendly : state->sprite_locations.beam_enemy);
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
    Image icon = LoadImage(GET_IMAGE_PATH("spaceship-icon.png"));

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

#ifdef DEBUG
    f32 xo = 5,
        yo = 5,
        wo = 100,
        ho = 100,
        thick = 2,
        font_size = 10,
        margin = 3,
        x = xo + thick + margin,
        y = yo + thick + margin;

    DrawRectangleLinesEx((Rectangle) { xo, yo, wo, ho }, thick, GREEN);
    DrawRectangle(xo + thick, yo + thick, wo - thick - thick, ho - thick - thick, (Color) { WHITE.r, WHITE.g, WHITE.b, 63 });

    DrawText("Debug info", x, y, font_size, WHITE);

    y += font_size + margin;
    DrawText(TextFormat("FPS: %d", GetFPS()), x, y, font_size, WHITE);

    y += font_size;
    DrawText(TextFormat("Beams [Total]: %lld", state->entities.beams.length), x, y, font_size, WHITE);

    y += font_size;
    DrawText(TextFormat("Beams [Alive]: %lld", state->entities.beams.alive_count), x, y, font_size, WHITE);
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
