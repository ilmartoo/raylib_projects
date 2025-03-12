#include <stdio.h>
#include <stdlib.h>

#include "arena.h"
#include "object_pool.h"
#include "debug.h"
#include "debug_panel.h"
#include "entities.h"
#include "files.h"
#include "inputs.h"
#include "memory_utils.h"
#include "rayheader.h"
#include "state.h"

// ----------------------------------------------------------------------------
// ---- Constants -------------------------------------------------------------
// ----------------------------------------------------------------------------

#define GAME_TITLE     "Spaceship"
#define GAME_ICON_FILE "spaceship-icon.png"

#define INITIAL_SCREEN_HEIGHT 1000
#define INITIAL_SCREEN_WIDTH  1600

// ----------------------------------------------------------------------------
// ---- Debug -----------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifdef DEBUG

#define DEBUG_PANEL_SCREEN_POSITION ((Vector2){20, 20})

DebugPanel *dbg_panels[2];

void debug_initialize(void)
{
    Font section_font = FontFromFamily(FONT_ATKINSON_BOLD);
    Font entry_font = FontFromFamily(FONT_ATKINSON_REGULAR);
    dbg_panels[0] = DebugPanelCreate(DARKGREEN, section_font, entry_font);
    dbg_panels[1] = DebugPanelCreate(GREEN, section_font, entry_font);
}

void debug_input(void)
{
    if (IsKeyPressed(KEY_P)) { state.time_running = !state.time_running; }

    bool increase_speed = IsKeyPressed(KEY_UP);
    bool decrease_speed = IsKeyPressed(KEY_DOWN);
    if (increase_speed != decrease_speed)
    {
        if (increase_speed && state.time_speed_magnitude < GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX) { ++state.time_speed_magnitude; }
        if (decrease_speed && state.time_speed_magnitude > -GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX) { --state.time_speed_magnitude; }
    }
}

void debug_update(void)
{
    DebugPanel *timings_panel = dbg_panels[0], *entities_panel = dbg_panels[1];
    DebugPanelClean(timings_panel);
    DebugPanelClean(entities_panel);

    DebugPanelAddTitle(timings_panel, "TIMINGS");
    DebugPanelAddEntry(timings_panel, TextFormat("%d fps", GetFPS()));
    DebugPanelAddEntry(timings_panel, TextFormat("%d%% speed", 100 + 20 * state.time_speed_magnitude));
    DebugPanelAddEntry(timings_panel, TextFormat("Game %s", state.time_running ? "running" : "paused"));

    ObjectPoolForEachObject(&state.players, Player, iter)
    {
        DebugPanelAddTitle(entities_panel, TextFormat("PLAYER %d", iter.index));
        DebugPanelAddEntry(entities_panel, TextFormat("Rotation: %.2f deg", RAD2DEG * iter.object->entity.rotation));
        DebugPanelAddEntry(entities_panel,
                           TextFormat("Velocity: (%.2f, %.2f)", iter.object->entity.velocity.x, iter.object->entity.velocity.y));
    }

    DebugPanelAddTitle(entities_panel, "PLAYER PROYECTILES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state.projectiles_players)));
    DebugPanelAddEntry(entities_panel, TextFormat(" Valid: %u", ObjectPoolObjectCount(state.projectiles_players)));

    DebugPanelAddTitle(entities_panel, "ENEMIES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state.enemies)));
    DebugPanelAddEntry(entities_panel, TextFormat(" Valid: %u", ObjectPoolObjectCount(state.enemies)));

    DebugPanelAddTitle(entities_panel, "ENEMY PROYECTILES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state.projectiles_enemies)));
    DebugPanelAddEntry(entities_panel, TextFormat(" Valid: %u", ObjectPoolObjectCount(state.projectiles_enemies)));
}

void debug_clear(void)
{
    DebugPanelDelete(dbg_panels[0]);
    DebugPanelDelete(dbg_panels[1]);
}

void debug_all()
{
    Vector2 timings_size = DebugPanelMeasures(*dbg_panels[0]);
    Vector2 timings_pos = DEBUG_PANEL_SCREEN_POSITION;
    Vector2 entities_pos = (Vector2){timings_pos.x, (timings_pos.y * 2) + timings_size.y};
    DebugPanelDraw(*dbg_panels[0], timings_pos);
    DebugPanelDraw(*dbg_panels[1], entities_pos);

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
    ObjectPool *pool;
    ObjectPoolForEachObject(pool = &state->projectiles_players, Projectile, iter)
    {
        if (!ProjectileMove(iter.object)) { ObjectPoolPop(pool, iter.index); }
    }

    ObjectPoolForEachObject(pool = &state->projectiles_enemies, Projectile, iter)
    {
        if (!ProjectileMove(iter.object)) { ObjectPoolPop(pool, iter.index); }
    }
}

void update_players(void)
{
    ObjectPoolForEachObject(&state->players, Player, iter)
    {
        PlayerMove(iter.object);
        PlayerAim(iter.object);
        PlayerShootBasic(iter.object);
        PlayerShootMissile(iter.object);
    }
}

void update_entities(void)
{
    update_players();
    update_proyectiles();
}

void update(void)
{
    GameStateUpdate();
    update_entities();
}

// ----------------------------------------------------------------------------
// ---- Drawing ---------------------------------------------------------------
// ----------------------------------------------------------------------------

void draw_players(void)
{
    ObjectPoolForEachObject(&state->players, Player, iter) { PlayerDraw(*iter.object); }
}

void draw_enemies(void)
{
    ObjectPoolForEachObject(&state->enemies, Enemy, iter) { EnemyDraw(*iter.object); }
}

void draw_proyectiles(void)
{
    ObjectPoolForEachObject(&state->projectiles_players, Projectile, iter) { ProjectileDraw(*iter.object); }
    ObjectPoolForEachObject(&state->projectiles_enemies, Projectile, iter) { ProjectileDraw(*iter.object); }
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

void entities_initialize(void)
{
    PlayerCreate(SPACESHIP_FRIENDLY_BASE, Vector2Scale(Vector2From(GetScreenWidth(), GetScreenHeight()), 0.5f));
}

void initialize(void)
{
    setup_window();

    SetTargetFPS(144);
    SetExitKey(KEY_NULL);

    GameStateInitialization();

#ifdef DEBUG
    debug_initialize();
#endif

    entities_initialize();
}

void clear(void)
{
    CloseWindow();
    GameStateCleanup();

#ifdef DEBUG
    debug_clear();
#endif
}

void loop(void)
{
    while (!WindowShouldClose()) // Detect window close button or defined exit key
    {
#ifdef DEBUG
        debug_input();
#endif

        if (state->time_running)
        {
            update();
            // check_collisions();
        }

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
