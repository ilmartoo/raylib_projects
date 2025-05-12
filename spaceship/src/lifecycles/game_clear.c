#include "game_debug.h"
#include "game_state.h"
#include "raylib.h"

// Game clear
void GameClear(void)
{
    CloseWindow();
    GameStateCleanup();

#ifdef DEBUG
    GameDebugClear();
#endif
}
