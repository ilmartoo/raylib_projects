#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "data_object_pool.h"
#include "debug.h"
#include "debug_panel.h"
#include "entities.h"
#include "files.h"
#include "memory_utils.h"
#include "raylib.h"
#include "state.h"
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
// Inputs
// :inputs :movement
//
#pragma region

bool proyectile_move(Projectile* projectile)
{
    if ((float)fabs(vector_distance(projectile->entity.position, projectile->origin)) < projectile->range) {
        projectile->entity.position.x -= adjust_to_delta(projectile->entity.movement_speed.x);
        projectile->entity.position.y -= adjust_to_delta(projectile->entity.movement_speed.y);
        return true;
    }
    return false;
}

void player_move(Player* player)
{
    Spaceship* ship = &player->spaceship;

    bool isLeft = IsKeyDown(player->controls.left);
    bool isRight = IsKeyDown(player->controls.right);
    bool isUp = IsKeyDown(player->controls.up);
    bool isDown = IsKeyDown(player->controls.down);

    if (isLeft != isRight) {
        f32 x = adjust_to_delta(ship->movement.speed.x);
        ship->pos.coords.x += isLeft ? -x : x;
    }
    if (isUp != isDown) {
        f32 y = adjust_to_delta(ship->movement.speed.y);
        ship->pos.coords.y += isUp ? -y : y;
    }
}

void player_shoot(Player* player)
{
    if (cooldown_up(player->shoot.cooldown)) {
        bool isShooting = IsKeyDown(player->controls.shoot);

        if (isShooting) {
            Spaceship ship = player->spaceship;
            Vector2 proyectile_pos = { ship.pos.coords.x + ((ship.pos.size.x - player->attack.proyectile_size.x) / 2),
                ship.pos.coords.y - player->attack.proyectile_size.y + (ship.pos.size.y / 2) };
            generate_player_proyectile(player->attack.damage, player->attack.range, player->attack.proyectile_speed,
                player->attack.proyectile_size, proyectile_pos);
            cooldown_update(&player->attack.cooldown);
        }
    }
}

void update_proyectiles(void)
{
    DataObjectPool* pool;
    data_object_pool_for_each_object(pool = &state.projectiles_players, Projectile, iter)
    {
        if (!proyectile_move(iter.data)) { data_object_pool_pop(pool, iter.index); }
    }

    data_object_pool_for_each_object(pool = &state.projectiles_enemies, Projectile, iter)
    {
        if (!proyectile_move(iter.data)) { data_object_pool_pop(pool, iter.index); }
    }
}

void update_players(void)
{
    data_object_pool_for_each_object(&state.players, Player, iter)
    {
        player_move(iter.data);
        player_shoot(iter.data);
    }
}

#pragma endregion

//
// Drawing
// :drawing
//
#pragma region

void draw_player(Player player)
{
    DrawTexturePro(state.spritesheet, locate_texture_spaceship(player.type),
        (Rectangle) { player.entity.position.x, player.entity.position.y, player.entity.size.x, player.entity.size.y },
        (Vector2) { 0 }, player.entity.rotation, WHITE);
}

void draw_enemy(Enemy enemy)
{
    DrawTexturePro(state.spritesheet, locate_texture_spaceship(enemy.type),
        (Rectangle) { enemy.entity.position.x, enemy.entity.position.y, enemy.entity.size.x, enemy.entity.size.y }, (Vector2) { 0 },
        enemy.entity.rotation, WHITE);
}

void draw_proyectile(Projectile projectile)
{
    DrawTexturePro(
        state.spritesheet, locate_texture_projectile(projectile.type),
        (Rectangle) { projectile.entity.position.x, projectile.entity.position.y, projectile.entity.size.x, projectile.entity.size.y },
        (Vector2) { 0 }, projectile.entity.rotation, WHITE);
}

void draw_players(void)
{
    data_object_pool_for_each_object(&state.players, Player, iter) { draw_player(*iter.data); }
}

void draw_enemies(void)
{
    data_object_pool_for_each_object(&state.enemies, Enemy, iter) { draw_enemy(*iter.data); }
}

void draw_proyectiles(void)
{
    data_object_pool_for_each_object(&state.projectiles_players, Projectile, iter) { draw_proyectile(*iter.data); }
    data_object_pool_for_each_object(&state.projectiles_enemies, Projectile, iter) { draw_proyectile(*iter.data); }
}

#pragma endregion

//
// Debug
// :debug
//
#pragma region

#define DEBUG_PANEL_SCREEN_POSITION ((Vector2) { 20, 20 })

DebugPanel* dbg_panels[2];

void debug_panel_initialize(void)
{
    Font section_font = font(FONT_ATKINSON_BOLD), entry_font = font(FONT_ATKINSON_REGULAR);
    dbg_panels[0] = debug_panel_create(DARKGREEN, section_font, entry_font);
    dbg_panels[1] = debug_panel_create(GREEN, section_font, entry_font);
}

void debug_panel_update(void)
{
    DebugPanel *timings_panel = dbg_panels[0], *entities_panel = dbg_panels[1];
    debug_panel_clean(timings_panel);
    debug_panel_clean(entities_panel);

    debug_panel_add_section(timings_panel, "TIMINGS");
    debug_panel_add_entry(timings_panel, TextFormat("%d fps", GetFPS()));

    debug_panel_add_section(entities_panel, "PLAYERS");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.players)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.players)));

    debug_panel_add_section(entities_panel, "PLAYER PROYECTILES");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.projectiles_players)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.projectiles_players)));

    debug_panel_add_section(entities_panel, "ENEMIES");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.enemies)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.enemies)));

    debug_panel_add_section(entities_panel, "ENEMY PROYECTILES");
    debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.projectiles_enemies)));
    debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.projectiles_enemies)));
}

void debug_panel_clear(void)
{
    debug_panel_delete(dbg_panels[0]);
    debug_panel_delete(dbg_panels[1]);
}

void debug_panel_draw_all()
{
    Vector2 timings_pos = DEBUG_PANEL_SCREEN_POSITION,
            entities_pos = (Vector2) { timings_pos.x, (timings_pos.y * 2) + debug_panel_measure(*dbg_panels[0]).y };
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
    const char* game_title = "Spaceship";
    Image icon = LoadImage(GET_IMAGE_PATH("spaceship-icon.png"));

    InitWindow(INIT_SCREEN_W, INIT_SCREEN_H, game_title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(icon);

    UnloadImage(icon);
}

void update_game_state(void)
{
    state.time_elapsed = GetTime();
    state.time_delta = GetFrameTime();
}

void update_entities(void)
{
    update_players();
    update_proyectiles();
}

void drawing(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    draw_players();
    draw_enemies();
    draw_proyectiles();

    IF_DEBUG { void debug_panel_draw_all(); }

    EndDrawing();
}

int main(void)
{
    SetTargetFPS(144);

    setup_window();
    game_state_initialize();

    IF_DEBUG { debug_panel_initialize(); }

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        update_game_state();
        update_entities();
        // check_collisions();

        IF_DEBUG { debug_panel_update(); }

        drawing();
    }

    CloseWindow();
    game_state_clear();

    IF_DEBUG { debug_panel_clear(); }

    return EXIT_SUCCESS;
}

#pragma endregion
