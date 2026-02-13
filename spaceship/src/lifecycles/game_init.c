#include <stdlib.h>
#include <time.h>

#include "utils/files.h"
#include "debug/game_debug.h"
#include "lifecycles/game_lifecycle.h"
#include "lifecycles/game_state.h"
#include "raylib/raylib.h"

#define GAME_TITLE     "Spaceship"
#define GAME_ICON_FILE "spaceship-icon.png"

void GameSetupWindow(void);
void GameInitializeEntities(void);

// Game initialization
void GameInitialize(void) {
    SetRandomSeed((u32)(time(NULL) % UINT_MAX));

    GameSetupWindow();
    GameStateInitialize();

#ifdef DEBUG
    GameDebugInitialize();
#endif

    GameInitializeEntities();
}

// Window setup
void GameSetupWindow(void) {
    i32 monitor_id = GetCurrentMonitor();
    Image icon = LoadImage(path_image(GAME_ICON_FILE));

    InitWindow(GetMonitorWidth(monitor_id), GetMonitorHeight(monitor_id), GAME_TITLE);
    SetWindowIcon(icon);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    MaximizeWindow();

    // DisableCursor(); // Locks cursor into the scren

    SetTargetFPS(GetMonitorRefreshRate(monitor_id));
    SetExitKey(KEY_NULL);

    UnloadImage(icon);
}

// Entities initialization
void GameInitializeEntities(void) { GameStatePlayerAdd(); }