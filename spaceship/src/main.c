#include <math.h>
#include "raylib.h"
#include "types.h"
#include "helpers.h"
#include "debug.h"
#include "arena.h"

//
// Configuration
// :config
//
#pragma region

#define ASSETS_DIR "assets/"
#define IMAGES_DIR ASSETS_DIR "images/"
#define GetImagePath(image_name) IMAGES_DIR image_name

#define INIT_SCREEN_H 600
#define INIT_SCREEN_W 1200

#pragma endregion

//
// Utilities
// :utils
//
#pragma region

#pragma endregion

//
// Spaceship things
// :spaceship
//
#pragma region

typedef struct Spaceship {
    f32 shoot_speed;
    f32 movement_speed;
    Vector2 size;
    Vector2 pos;
} Spaceship;

void drawSpaceship(Spaceship sship)
{
    DrawTriangle(
        (Vector2){ .x = sship.pos.x, .y = sship.size.y + sship.pos.y},
        (Vector2){ .x = sship.size.x + sship.pos.x, .y = sship.size.y + sship.pos.y},
        (Vector2){ .x = (sship.size.x / 2) + sship.pos.x, .y = sship.pos.y},
        GREEN
    );
}

#pragma endregion

//
// Input handling
// :input
//
#pragma region

void handleKeyPress(f32 delta)
{
    // TODO move spaceship
}

#pragma endregion

//
// Main function
// :main
//
#pragma region

int main()
{
    string gameTitle = "Spaceship";

    InitWindow(INIT_SCREEN_W, INIT_SCREEN_H, gameTitle);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(LoadImage(GetImagePath("snake-icon.png")));
    SetTargetFPS(60);

    Spaceship sship = {
        .shoot_speed = 0,
        .movement_speed = 10
        .size = (Vector2){ .x = 20, .y = 20 },
        .pos = (Vector2){ .x = 10, .y = 10 }
    };

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // DRAWING SECTION
        BeginDrawing();
        // ------------------------

        ClearBackground(DARKGRAY);

        drawSpaceship(sship);

        DrawFPS(0, 0);

        // ------------------------
        EndDrawing();


    }

    CloseWindow();

    return 0;
}

#pragma endregion
