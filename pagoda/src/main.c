#include <math.h>

#include "debug.h"
#include "helpers.h"
#include "raylib.h"
#include "types.h"

#include "index.h"

int main()
{
    string game_title;
#ifdef DEBUG
    game_title = "Pagoda game - Debug";
#else
    game_title = "Pagoda game";
#endif



    




    // Initialization
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, game_title);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(LoadImage(GetImage("pagoda.jpg")));

    SetTargetFPS(60); // Set our game to run at 60 frames-per-second

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        Draw({
            ClearBackground(DARKGRAY); // Set background color

            DEBUG_BLOCK({
                DrawFPS(10, 10);
            });
        });
    }

    // De-Initialization
    CloseWindow(); // Close window and OpenGL context

    return 0;
}
