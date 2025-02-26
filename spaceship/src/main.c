#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "data_object_pool.h"
#include "debug.h"
#include "debug_panel.h"
#include "files.h"
#include "memory_utils.h"
#include "raylib.h"
#include "types.h"

//
// Configuration
// :macros
//
#pragma region

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

typedef enum ProyectileType
{
    PROYECTILE_PLAYER = 0,
#define PROYECTILE_PLAYER_TEXTURE_POS ((Rectangle){608, 0, 64, 127})
    PROYECTILE_ENEMY,
#define PROYECTILE_ENEMY_TEXTURE_POS ((Rectangle){688, 160, 64, 127})
} ProyectileType;

typedef struct Proyectile
{
    u32 damage;
    f32 range;
    Vector2 origin;
    /* ... */
    ProyectileType type;
    Movable movement;
    Positional pos;
} Proyectile;

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

typedef enum FontFamily
{
    FONT_ATKINSON_REGULAR = 0,
    FONT_ATKINSON_BOLD,
} FontFamily;

typedef struct GameState
{
    struct
    {
        f64 elapsed;
        f32 delta;
    } time;
    /* ... */
    struct
    {
        DataObjectPool players;
        DataObjectPool enemies;
        DataObjectPool player_proyectiles;
        DataObjectPool enemy_proyectiles;
    } pools;
    /* ... */
    struct
    {
        Texture2D texture;
        Rectangle spaceships[4];
        Rectangle proyectiles[2];
    } spritesheet;
    /* ... */
    Font fonts[2];
} GameState;

GameState state = {0};

#pragma endregion

//
// Util Functions And Macros
// :utils :functions :macros
//
#pragma region

Rectangle spaceship_texture_loc(SpaceshipType type) { return state.spritesheet.spaceships[type]; }

Rectangle proyectile_texture_loc(ProyectileType type) { return state.spritesheet.proyectiles[type]; }

Font *font(FontFamily type) { return &state.fonts[type]; }

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

Proyectile create_proyectile(ProyectileType type, f32 damage, f32 range, f32 movement_speed, Vector2 size, Vector2 pos)
{
    return (Proyectile){.damage = damage,
                        .range = range,
                        .origin = pos,
                        .type = type,
                        .movement = {.speed = {0, movement_speed}},
                        .pos = {.size = size, .coords = pos}};
}

Proyectile *generate_player_proyectile(f32 damage, f32 range, f32 movement_speed, Vector2 size, Vector2 pos)
{
    Proyectile proyectile = create_proyectile(PROYECTILE_PLAYER, damage, range, movement_speed, size, pos);
    return data_object_pool_add(&state.pools.player_proyectiles, &proyectile);
}

Proyectile *generate_enemy_proyectile(f32 damage, f32 range, f32 movement_speed, Vector2 size, Vector2 pos)
{
    Proyectile proyectile = create_proyectile(PROYECTILE_ENEMY, damage, range, movement_speed, size, pos);
    return data_object_pool_add(&state.pools.enemy_proyectiles, &proyectile);
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
    return data_object_pool_add(&state.pools.players, &player);
}

void global_state_init(void)
{
    Image spritesheet_image = LoadImage(GET_IMAGE_PATH("spritesheet.png"));

    state = (GameState){
        .time = {.elapsed = GetTime(), .delta = GetFrameTime()},
        .pools = {.players = data_object_pool_create_type(Player),
                  .enemies = data_object_pool_create_type(Enemy),
                  .player_proyectiles = data_object_pool_create_type(Proyectile),
                  .enemy_proyectiles = data_object_pool_create_type(Proyectile)},
        .spritesheet = {.texture = LoadTextureFromImage(spritesheet_image),
                        .spaceships = {SPACESHIP_FRIENDLY_BASE_TEXTURE_POS, SPACESHIP_FRIENDLY_UPGRADED_TEXTURE_POS,
                                       SPACESHIP_ENEMY_BASE_TEXTURE_POS, SPACESHIP_ENEMY_UPGRADED_TEXTURE_POS},
                        .proyectiles = {PROYECTILE_PLAYER_TEXTURE_POS, PROYECTILE_ENEMY_TEXTURE_POS}},
        .fonts = {LoadFont(GET_FONT_PATH("atkinson_hyper_mono_regular.ttf")), LoadFont(GET_FONT_PATH("atkinson_hyper_mono_bold.ttf"))}};

    UnloadImage(spritesheet_image);

    generate_entity_player((Vector2){GetScreenWidth() / 2, GetScreenHeight() / 2});
}

void global_state_clear(void)
{
    data_object_pool_release(&state.pools.players);
    data_object_pool_release(&state.pools.enemies);
    data_object_pool_release(&state.pools.player_proyectiles);
    data_object_pool_release(&state.pools.enemy_proyectiles);
    UnloadTexture(state.spritesheet.texture);
    UnloadFont(state.fonts[0]);
    UnloadFont(state.fonts[1]);
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

bool proyectile_entity_move(Proyectile *b)
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
            Vector2 proyectile_pos = {ship.pos.coords.x + ((ship.pos.size.x - player->attack.proyectile_size.x) / 2),
                                      ship.pos.coords.y - player->attack.proyectile_size.y + (ship.pos.size.y / 2)};
            generate_player_proyectile(player->attack.damage, player->attack.range, player->attack.proyectile_speed,
                                       player->attack.proyectile_size, proyectile_pos);
            cooldown_update(&player->attack.cooldown);
        }
    }
}

void update_proyectile_entities(void)
{
    DataObjectPool *pool;
    data_object_pool_for_each_object(pool = &state.pools.player_proyectiles, Proyectile, itr)
    {
        if (!proyectile_entity_move(itr.data_object)) { data_object_pool_pop(pool, itr.index); }
    }

    data_object_pool_for_each_object(pool = &state.pools.enemy_proyectiles, Proyectile, itr)
    {
        if (!proyectile_entity_move(itr.data_object)) { data_object_pool_pop(pool, itr.index); }
    }
}

void update_player_entities(void)
{
    data_object_pool_for_each_object(&state.pools.players, Player, itr)
    {
        player_entity_move(itr.data_object);
        player_entity_shoot(itr.data_object);
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
    DrawTexturePro(state.spritesheet.texture, spaceship_texture_loc(s.type),
                   (Rectangle){s.pos.coords.x, s.pos.coords.y, s.pos.size.x, s.pos.size.y}, (Vector2){0}, 0, WHITE);
}

void draw_proyectile(Proyectile b)
{
    DrawTexturePro(state.spritesheet.texture, proyectile_texture_loc(b.type),
                   (Rectangle){b.pos.coords.x, b.pos.coords.y, b.pos.size.x, b.pos.size.y}, (Vector2){0}, 0, WHITE);
}

void draw_player_entities(void)
{
    data_object_pool_for_each_object(&state.pools.players, Player, itr) { draw_spaceship(itr.data_object->spaceship); }
}

void draw_enemy_entities(void)
{
    data_object_pool_for_each_object(&state.pools.enemies, Enemy, itr) { draw_spaceship(itr.data_object->spaceship); }
}

void draw_proyectile_entities(void)
{
    data_object_pool_for_each_object(&state.pools.player_proyectiles, Proyectile, itr) { draw_proyectile(*itr.data_object); }
    data_object_pool_for_each_object(&state.pools.enemy_proyectiles, Proyectile, itr) { draw_proyectile(*itr.data_object); }
}

#pragma endregion

//
// Debug
// :debug
//
#pragma region

#define DEBUG_PANEL_SCREEN_POSITION ((Vector2){20, 20})

DebugPanel *dbg_panels[2];

void debug_panel_init()
{
    Font *sf = font(FONT_ATKINSON_BOLD), *ef = font(FONT_ATKINSON_REGULAR);
    dbg_panels[0] = debug_panel_create(DARKGREEN, sf, ef);
    dbg_panels[1] = debug_panel_create(GREEN, sf, ef);
}

void debug_panel_update()
{
    DebugPanel *timings_panel = dbg_panels[0], *entities_panel = dbg_panels[1];
    debug_panel_clean(timings_panel);
    debug_panel_clean(entities_panel);

    debug_panel_add_section(timings_panel, NULL);
    debug_panel_add_entry(timings_panel, TextFormat("%d fps", GetFPS()));

    debug_panel_add_section(entities_panel, "PLAYERS");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.pools.players)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.pools.players)));

    debug_panel_add_section(entities_panel, "PLAYER PROYECTILES");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.pools.player_proyectiles)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.pools.player_proyectiles)));

    debug_panel_add_section(entities_panel, "ENEMIES");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.pools.enemies)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.pools.enemies)));

    debug_panel_add_section(entities_panel, "ENEMY PROYECTILES");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.pools.enemy_proyectiles)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.pools.enemy_proyectiles)));
}

void debug_panel_clear()
{
    debug_panel_delete(dbg_panels[0]);
    debug_panel_delete(dbg_panels[1]);
}

void draw_debug_panel()
{
    Vector2 timings_pos = DEBUG_PANEL_SCREEN_POSITION,
            entities_pos = (Vector2){timings_pos.x, (timings_pos.y * 2) + debug_panel_measure(*dbg_panels[0]).y};
    debug_panel_draw(*dbg_panels[0], timings_pos);
    debug_panel_draw(*dbg_panels[1], entities_pos);
}

#pragma endregion

//
// Main function
// :main
//
#pragma region

void setup_window(void)
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
    update_proyectile_entities();
}

void do_drawing(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    draw_player_entities();
    draw_enemy_entities();
    draw_proyectile_entities();

    IF_DEBUG { draw_debug_panel(); }

    EndDrawing();
}

int main(void)
{
    SetTargetFPS(144);

    setup_window();
    global_state_init();

    IF_DEBUG { debug_panel_init(); }

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        update_times();
        update_entities();
        // check_collisions();

        IF_DEBUG { debug_panel_update(); }

        do_drawing();
    }

    CloseWindow();
    global_state_clear();

    IF_DEBUG { debug_panel_clear(); }

    return EXIT_SUCCESS;
}

#pragma endregion
