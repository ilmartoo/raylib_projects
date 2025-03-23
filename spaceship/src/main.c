#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "arena.h"
#include "collisions.h"
#include "debug.h"
#include "debug_panel.h"
#include "entities.h"
#include "files.h"
#include "inputs.h"
#include "memory_utils.h"
#include "object_pool.h"
#include "rayheader.h"
#include "state.h"

// ----------------------------------------------------------------------------
// ---- Constants -------------------------------------------------------------
// ----------------------------------------------------------------------------

#define GAME_TITLE     "Spaceship"
#define GAME_ICON_FILE "spaceship-icon.png"

// ----------------------------------------------------------------------------
// ---- Testing ---------------------------------------------------------------
// ----------------------------------------------------------------------------

void GameDebugGenerateEnemiesAroundPlayer(void)
{
    ForEachObjectPoolObject(&state->enemies, Enemy, iter) { ObjectPoolPop(&state->enemies, iter.index); } // Might remove later on

#define ENEMIES_CREATED_AT_START         4
#define ENEMIES_CREATED_AT_START_SPACING 400

    Vector2 player_center = EntityCenter(state->players[0].entity);

    for (u32 i = 0; i < ENEMIES_CREATED_AT_START; ++i)
    {
        f32 rotation = Deg2Rad(GetRandomValue(0, MAX_DEGS));
        Vector2 pos = Vector2Add(player_center, Vector2Scale(Vector2UnitCirclePoint(rotation), ENEMIES_CREATED_AT_START_SPACING));

        EnemyCreate(i & 1 ? SPACESHIP_ENEMY_UPGRADED : SPACESHIP_ENEMY_BASE,
                    pos,
                    ENEMY_MOVEMENT_SPEED,
                    Vector2AngleFromXAxis(Vector2Subtract(player_center, pos)));
    }
}

void TestingInput(void)
{
    // Pause time (PAUSE ?)
    if (IsKeyPressed(KEY_ZERO)) { state->time_running = !state->time_running; }

    // Generate random enemies around player 0
    if (IsKeyPressed(KEY_ONE)) { GameDebugGenerateEnemiesAroundPlayer(); }

    // Toggle bounding circles
    if (IsKeyPressed(KEY_TWO)) { state->testing_draw_bounding_circles = !state->testing_draw_bounding_circles; }

    // Toggle player rotation
    if (IsKeyPressed(KEY_THREE)) { state->testing_draw_player_rotation = !state->testing_draw_player_rotation; }

    // Toggle maximized window
    if (IsKeyPressed(KEY_SEVEN)) { MaximizeWindow(); }

    // Toggle borderless window
    if (IsKeyPressed(KEY_EIGHT)) { ToggleBorderlessWindowed(); }

    // Toggle fullscreen
    if (IsKeyPressed(KEY_NINE)) { ToggleFullscreen(); }

    // Game speed
    bool increase_speed = IsKeyPressed(KEY_PAGE_UP);
    bool decrease_speed = IsKeyPressed(KEY_PAGE_DOWN);
    if (increase_speed != decrease_speed)
    {
        if (increase_speed && state->time_speed_magnitude < GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX) { ++state->time_speed_magnitude; }
        if (decrease_speed && state->time_speed_magnitude > -GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX)
        {
            --state->time_speed_magnitude;
        }
    }
}

// ----------------------------------------------------------------------------
// ---- Debug -----------------------------------------------------------------
// ----------------------------------------------------------------------------
#ifdef DEBUG

#define DEBUG_PANEL_SCREEN_POSITION ((Vector2){20, 20})

DebugPanel *dbg_panels[2];

// Debug panels initialization
void GameDebugInitialize(void)
{
    dbg_panels[0] = DebugPanelCreate(DARKGREEN, state->font);
    dbg_panels[1] = DebugPanelCreate(GREEN, state->font);
}

// Debug input check
void GameDebugInput(void) { /* Empty for now */ }

// Debug update
void GameDebugUpdate(void)
{
    DebugPanel *timings_panel = dbg_panels[0], *entities_panel = dbg_panels[1];
    DebugPanelClean(timings_panel);
    DebugPanelClean(entities_panel);

    DebugPanelAddTitle(timings_panel, "TIMINGS");
    DebugPanelAddEntry(timings_panel, TextFormat("%d fps", GetFPS()));
    DebugPanelAddEntry(timings_panel, TextFormat("%d%% speed", 100 + 20 * state->time_speed_magnitude));
    DebugPanelAddEntry(timings_panel, TextFormat("Game %s", state->time_running ? "running" : "paused"));

    ForEachPlayerVal(iter)
    {
        DebugPanelAddTitle(dbg_panels[1], TextFormat("PLAYER %d", iter.index));
        DebugPanelAddEntry(dbg_panels[1], TextFormat("Rotation: %.2f deg", Rad2Deg(iter.player.entity.rotation)));

        Vector2 velocity = iter.player.entity.velocity;
        DebugPanelAddEntry(dbg_panels[1], TextFormat("Velocity: (%.2f, %.2f)", velocity.x, velocity.y));
    }

    DebugPanelAddTitle(entities_panel, "PLAYER PROYECTILES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state->projectiles_players)));
    DebugPanelAddEntry(entities_panel, TextFormat("Valid: %u", ObjectPoolObjectCount(state->projectiles_players)));

    DebugPanelAddTitle(entities_panel, "ENEMIES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state->enemies)));
    DebugPanelAddEntry(entities_panel, TextFormat("Valid: %u", ObjectPoolObjectCount(state->enemies)));

    DebugPanelAddTitle(entities_panel, "ENEMY PROYECTILES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state->projectiles_enemies)));
    DebugPanelAddEntry(entities_panel, TextFormat("Valid: %u", ObjectPoolObjectCount(state->projectiles_enemies)));
}

// Debug clear
void GameDebugClear(void)
{
    DebugPanelDelete(dbg_panels[0]);
    DebugPanelDelete(dbg_panels[1]);
}

// Debug draw
void GameDebugDraw()
{
    Vector2 timings_size = DebugPanelMeasures(*dbg_panels[0]);
    Vector2 timings_pos = DEBUG_PANEL_SCREEN_POSITION;
    Vector2 entities_pos = (Vector2){timings_pos.x, (timings_pos.y * 2) + timings_size.y};
    DebugPanelDraw(*dbg_panels[0], timings_pos);
    DebugPanelDraw(*dbg_panels[1], entities_pos);
}
#endif

// ----------------------------------------------------------------------------
// ---- Update ----------------------------------------------------------------
// ----------------------------------------------------------------------------

// Update projectiles
void GameUpdateProyectiles(void)
{
    ObjectPool *pool;
    ForEachObjectPoolObject(pool = &state->projectiles_players, Projectile, iter)
    {
        if (!ProjectileMove(iter.object)) { ObjectPoolPop(pool, iter.index); }
    }

    ForEachObjectPoolObject(pool = &state->projectiles_enemies, Projectile, iter)
    {
        if (!ProjectileMove(iter.object)) { ObjectPoolPop(pool, iter.index); }
    }
}

// Update players
void GameUpdatePlayers(void)
{
    ForEachPlayerRef(iter)
    {
        Player *player = iter.player;
        if (!GameStateIsPlayerGameOver(*player))
        {
            PlayerMove(player);
            PlayerAim(player);
            PlayerShootBasic(player);
            PlayerShootMissile(player);
        }
    }
}

// Game update
void GameUpdate(void)
{
    GameStateUpdate();

    /* Entities */
    GameUpdatePlayers();
    GameUpdateProyectiles();
}

// Game collision checking
void GameCheckCollisions(void)
{
    ObjectPool *enemies = &state->enemies, *projectiles_enemies = &state->projectiles_enemies,
               *projectiles_players = &state->projectiles_players;

    ForEachObjectPoolObject(projectiles_enemies, Projectile, iter_projectile)
    {
        bool collided = false;
        Projectile projectile = *iter_projectile.object;
        ForEachPlayerRef(iter_player)
        {
            Player *player = iter_player.player;
            if (CheckEntityCollision(player->entity, projectile.entity))
            {
                PlayerDamage(player, projectile.damage);
                collided = true;
            }
        }
        if (collided) { ObjectPoolPop(projectiles_enemies, iter_projectile.index); }
    }

    ForEachObjectPoolObject(projectiles_players, Projectile, iter_projectile)
    {
        bool collided = false;
        Projectile projectile = *iter_projectile.object;
        ForEachObjectPoolObject(enemies, Enemy, iter_enemy)
        {
            Enemy *enemy = iter_enemy.object;
            if (CheckEntityCollision(enemy->entity, projectile.entity))
            {
                if (EnemyDamage(enemy, projectile.damage)) { ObjectPoolPop(enemies, iter_enemy.index); }
                collided = true;
            }
        }
        if (collided) { ObjectPoolPop(projectiles_players, iter_projectile.index); }
    }
}

// ----------------------------------------------------------------------------
// ---- Drawing ---------------------------------------------------------------
// ----------------------------------------------------------------------------

// Player drawing
void GameDrawPlayers(void)
{
    ForEachPlayerVal(iter) { PlayerDraw(iter.player); }
}

// Enemy drawing
void GameDrawEnemies(void)
{
    ForEachObjectPoolObject(&state->enemies, Enemy, iter) { EnemyDraw(*iter.object); }
}

// Projectile drawing
void GameDrawProyectiles(void)
{
    ForEachObjectPoolObject(&state->projectiles_players, Projectile, iter) { ProjectileDraw(*iter.object); }
    ForEachObjectPoolObject(&state->projectiles_enemies, Projectile, iter) { ProjectileDraw(*iter.object); }
}

// Game draw
void GameDraw(void)
{
    /* Start */ BeginDrawing();

    ClearBackground(BLANK);

    GameDrawPlayers();
    GameDrawEnemies();
    GameDrawProyectiles();

#ifdef DEBUG
    GameDebugDraw();
#else
    DrawFPS(10, 10);
#endif

    /* End */ EndDrawing();
}

// ----------------------------------------------------------------------------
// ---- Game loop --------------------------------------------------------------
// ----------------------------------------------------------------------------

// Window setup
void GameSetupWindow(void)
{
    i32 monitor_id = GetCurrentMonitor();
    Image icon = LoadImage(path_image(GAME_ICON_FILE));

    InitWindow(GetMonitorWidth(monitor_id), GetMonitorHeight(monitor_id), GAME_TITLE);
    SetWindowIcon(icon);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();

    DisableCursor(); // Locks cursor into the scren

    SetTargetFPS(GetMonitorRefreshRate(monitor_id));
    SetExitKey(KEY_NULL);

    UnloadImage(icon);
}

// Entities initialization
void GameInitializeEntities(void)
{
    GameStatePlayerAdd();

#ifdef DEBUG
    GameDebugGenerateEnemiesAroundPlayer();
#endif
}

// Game initialization
void GameInitialize(void)
{
    GameSetupWindow();

    SetRandomSeed((u32)(time(NULL) % UINT_MAX));

    GameStateInitialization();

#ifdef DEBUG
    GameDebugInitialize();
#endif

    GameInitializeEntities();
}

// Game clear
void GameClear(void)
{
    CloseWindow();
    GameStateCleanup();

#ifdef DEBUG
    GameDebugClear();
#endif
}

// Game loop
void GameLoop(void)
{
    while (!WindowShouldClose()) // Detect window close button or defined exit key
    {
#ifdef DEBUG
        GameDebugInput();
#endif
        // TESTING PURPOSSES ///
        TestingInput();
        /////////////////

        if (state->time_running)
        {
            GameUpdate();
            GameCheckCollisions();
        }

#ifdef DEBUG
        GameDebugUpdate();
#endif

        GameDraw();
    }
}

// ----------------------------------------------------------------------------
// ---- Main ------------------------------------------------------------------
// ----------------------------------------------------------------------------

i32 main(void)
{
    GameInitialize(); // Game initialization
    GameLoop();       // Game loop
    GameClear();      // Game cleaning

    return EXIT_SUCCESS;
}
