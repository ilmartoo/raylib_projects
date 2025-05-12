#include "game_lifecycle.h"
#include "raylib.h"

// Check if the game should end
bool GameShouldClose(void) { return WindowShouldClose(); }

// The game loop
void GameLoop(void)
{
    while (!GameShouldClose()) // Detect window close button or defined exit key
    {
        GameFrame();
        GameDraw();
    }
}