#include "debug/game_debug.h"
#include "lifecycles/game_lifecycle.h"
#include "lifecycles/game_state.h"
#include "raylib/raylib.h"

// Game clear
void GameClear(void) {
    CloseWindow();
    GameStateCleanup();

#ifdef DEBUG
    GameDebugClear();
#endif
}
