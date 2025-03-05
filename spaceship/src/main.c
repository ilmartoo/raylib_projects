#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "data_object_pool.h"
#include "debug.h"
#include "debug_panel.h"
#include "entities.h"
#include "files.h"
#include "inputs.h"
#include "memory_utils.h"
#include "raylib.h"
#include "raymath.h"
#include "state.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Constants -------------------------------------------------------------
// ----------------------------------------------------------------------------

#define GAME_TITLE     "Spaceship"
#define GAME_ICON_FILE "spaceship-icon.png"

#define INITIAL_SCREEN_HEIGHT 600
#define INITIAL_SCREEN_WIDTH  1200

// ----------------------------------------------------------------------------
// ---- Debug -----------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifdef DEBUG

#define DEBUG_PANEL_SCREEN_POSITION ((Vector2){20, 20})

DebugPanel *dbg_panels[2];

void debug_initialize(void)
{
    Font section_font = font(FONT_ATKINSON_BOLD), entry_font = font(FONT_ATKINSON_REGULAR);
    dbg_panels[0] = debug_panel_create(DARKGREEN, section_font, entry_font);
    dbg_panels[1] = debug_panel_create(GREEN, section_font, entry_font);
}

void debug_update(void)
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

    data_object_pool_for_each_object(&state.players, Player, iter)
    {
        debug_panel_add_section(entities_panel, TextFormat("PLAYER %d", iter.index));
        debug_panel_add_entry(entities_panel, TextFormat("Rotation: %.2f deg", RAD2DEG * iter.data->entity.rotation));
        debug_panel_add_entry(entities_panel,
                              TextFormat("Movement: X [%.2f, %.2f] Y [%.2f, %.2f]", player_input(*iter.data, ACTION_MOVE_DOWN),
                                         player_input(*iter.data, ACTION_MOVE_UP), player_input(*iter.data, ACTION_MOVE_LEFT),
                                         player_input(*iter.data, ACTION_MOVE_RIGHT)));
        debug_panel_add_entry(entities_panel,
                              TextFormat("Velocity: (%.2f, %.2f)", iter.data->entity.velocity.x, iter.data->entity.velocity.y));
    }

    // debug_panel_add_section(entities_panel, "ENEMIES");
    // debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.enemies)));
    // debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.enemies)));

    // debug_panel_add_section(entities_panel, "ENEMY PROYECTILES");
    // debug_panel_add_entry(entities_panel, TextFormat("Chunks: %u", data_object_pool_chunk_count(state.projectiles_enemies)));
    // debug_panel_add_entry(entities_panel, TextFormat(" Valid: %u", data_object_pool_object_count(state.projectiles_enemies)));
}

void debug_clear(void)
{
    debug_panel_delete(dbg_panels[0]);
    debug_panel_delete(dbg_panels[1]);
}

void debug_all()
{
    Vector2 timings_size = debug_panel_measure(*dbg_panels[0]);
    Vector2 timings_pos = DEBUG_PANEL_SCREEN_POSITION;
    Vector2 entities_pos = (Vector2){timings_pos.x, (timings_pos.y * 2) + timings_size.y};
    debug_panel_draw(*dbg_panels[0], timings_pos);
    debug_panel_draw(*dbg_panels[1], entities_pos);

    // f32 rad = 0;
    // u32 spacing = 100;
    // u32 size = 150;
    // u32 base_line_x = 100, base_line_y = 200;
    // for (u32 i = 0; i < 8; ++i, rad += PI * 0.25 * i) { DrawLine(, YELLOW); }
}
#endif

// ----------------------------------------------------------------------------
// ---- Update ----------------------------------------------------------------
// ----------------------------------------------------------------------------

void update_proyectiles(void)
{
    DataObjectPool *pool;
    data_object_pool_for_each_object(pool = &state.projectiles_players, Projectile, iter)
    {
        if (!projectile_move(iter.data)) { data_object_pool_pop(pool, iter.index); }
    }

    data_object_pool_for_each_object(pool = &state.projectiles_enemies, Projectile, iter)
    {
        if (!projectile_move(iter.data)) { data_object_pool_pop(pool, iter.index); }
    }
}

void update_players(void)
{
    data_object_pool_for_each_object(&state.players, Player, iter)
    {
        player_move(iter.data);
        player_aim(iter.data);
        player_shoot(iter.data);
        player_shoot_missile(iter.data);
    }
}

void update_entities(void)
{
    update_players();
    update_proyectiles();
}

void update_game_state(void)
{
    state.time_elapsed = GetTime();
    state.time_delta = GetFrameTime();
}

void update(void)
{
    update_game_state();
    update_entities();
}

// ----------------------------------------------------------------------------
// ---- Drawing ---------------------------------------------------------------
// ----------------------------------------------------------------------------

void draw_players(void)
{
    data_object_pool_for_each_object(&state.players, Player, iter) { player_draw(*iter.data); }
}

void draw_enemies(void)
{
    data_object_pool_for_each_object(&state.enemies, Enemy, iter) { enemy_draw(*iter.data); }
}

void draw_proyectiles(void)
{
    data_object_pool_for_each_object(&state.projectiles_players, Projectile, iter) { proyectile_draw(*iter.data); }
    data_object_pool_for_each_object(&state.projectiles_enemies, Projectile, iter) { proyectile_draw(*iter.data); }
}

void draw(void)
{
    BeginDrawing();
    ClearBackground(BLACK);

    draw_players();
    draw_enemies();
    draw_proyectiles();

#ifdef DEBUG
    debug_all();
#else
    DrawFPS(10, 10);
#endif

    EndDrawing();
}

// ----------------------------------------------------------------------------
// ---- Game loop --------------------------------------------------------------
// ----------------------------------------------------------------------------

void setup_window(void)
{
    Image icon = LoadImage(path_image(GAME_ICON_FILE));

    InitWindow(INITIAL_SCREEN_WIDTH, INITIAL_SCREEN_HEIGHT, GAME_TITLE);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(icon);

    UnloadImage(icon);
}

void entities_initialize(void) { player_create(SPACESHIP_FRIENDLY_BASE, (Vector2){GetScreenWidth() / 2.f, GetScreenHeight() / 2.f}); }

void initialize(void)
{
    setup_window();

    SetTargetFPS(144);
    SetExitKey(KEY_NULL);

    game_state_initialize();

#ifdef DEBUG
    debug_initialize();
#endif

    entities_initialize();
}

void clear(void)
{
    CloseWindow();
    game_state_clear();

#ifdef DEBUG
    debug_clear();
#endif
}

void loop(void)
{
    while (!WindowShouldClose()) // Detect window close button or defined exit key
    {
        update();
        // check_collisions();

#ifdef DEBUG
        debug_update();
#endif

        draw();
    }
}

// ----------------------------------------------------------------------------
// ---- Main ------------------------------------------------------------------
// ----------------------------------------------------------------------------

i32 main(void)
{
    initialize(); // Game initialization
    loop();       // Game loop
    clear();      // Game cleaning

    return EXIT_SUCCESS;
}
