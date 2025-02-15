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
#define GET_IMAGE_PATH(image_name) IMAGES_DIR image_name

#define INIT_SCREEN_H 600
#define INIT_SCREEN_W 1200

#define INIT_BOARD_H 40
#define INIT_BOARD_W 120
// #define INIT_BOARD_H 3
// #define INIT_BOARD_W 2
#define INIT_BOARD_TOTAL_ELEMENTS (INIT_BOARD_H * INIT_BOARD_W)

#define INIT_STEP_MILLISECONDS 1000

#pragma endregion

//
// Util types
// :util-types
//
#pragma region

typedef struct BoardPrintData {
    u32 offsetX;
    u32 offsetY;
    u32 cellSize;
} BoardPrintData;

#pragma endregion

//
// Board things
// :board
//
#pragma region

typedef struct Board {
    bool* cells;
    u32 height;
    u32 width;
} Board;

void drawCell(u32 x, u32 y, u32 size, bool active)
{
    u32 realSize = size - 1;
    DrawRectangle(x + 1, y + 1, realSize, realSize, active ? GOLD : LIGHTGRAY);
}

void drawBoard(Board board, BoardPrintData pData)
{
    u32 boardSize = board.height * board.width;

    for (u32 i = 0; i < boardSize; ++i) {
        u32 x = pData.offsetX + (i % board.width) * pData.cellSize,
            y = pData.offsetY + (i / board.width) * pData.cellSize;
        drawCell(x, y, pData.cellSize, board.cells[i]);
        // log_info("Cell %u [X=%u,Y=%u]\n", i, x, y);
    }
}

u8 cellAliveNeighbours(Board board, u32 cellIndex)
{
    u8 n = 0;

    u32 row = cellIndex / board.width,
        col = cellIndex % board.width;

    i64 leftCol = col - 1,
        rightCol = col + 1,
        topRow = row - 1,
        bottomRow = row + 1;

    if (leftCol >= 0) {
        // Top left
        if (topRow >= 0 && board.cells[leftCol + topRow * board.width]) { ++n; }
        // Middle left
        if (board.cells[leftCol + row * board.width]) { ++n; }
        // Bottom left
        if (bottomRow < board.height && board.cells[leftCol + bottomRow * board.width]) { ++n; }
    }

    // Top center
    if (row > 0 && board.cells[cellIndex - board.width]) { ++n; }
    // Bottom center
    if (row < board.height - 1 && board.cells[cellIndex + board.width]) { ++n; }

    if (rightCol < board.width) {
        // Top right
        if (topRow >= 0 && board.cells[rightCol + topRow * board.width]) { ++n; }
        // Middle right
        if (board.cells[rightCol + row * board.width]) { ++n; }
        // Bottom right
        if (bottomRow < board.height && board.cells[rightCol + bottomRow * board.width]) { ++n; }
    }

    return n;
}

void stepBoard(Board board)
{
    u32 boardSize = board.width * board.height;
    for (u32 i = 0; i < boardSize; ++i) {
        /**
         * Rules
         *
         * For a space that is populated:
         * - Each cell with one or no neighbors dies, as if by solitude.
         * - Each cell with four or more neighbors dies, as if by overpopulation.
         * - Each cell with two or three neighbors survives.
         *
         * For a space that is empty or unpopulated:
         * - Each cell with three neighbors becomes populated.
         *
         */
        u8 active = cellAliveNeighbours(board, i);
        board.cells[i] = active == 3 || (board.cells[i] && active == 2);
    }
}

#pragma endregion

//
// Util functions
// :util-functions
//
#pragma region

BoardPrintData getCellSize(Board board)
{
    u32 height = (u32)GetScreenHeight(),
        width = (u32)GetScreenWidth(),
        size = min(height / board.height, width / board.width);

    return (BoardPrintData) {
        .offsetX = (width - (board.width * size)) / 2,
        .offsetY = (height - (board.height * size)) / 2,
        .cellSize = size
    };
}

#pragma endregion

//
// Main function
// :main
//
#pragma region

int main()
{
    string gameTitle = "Game of life";

    InitWindow(INIT_SCREEN_W, INIT_SCREEN_H, gameTitle);
    SetWindowState(FLAG_WINDOW_RESIZABLE);
    SetWindowIcon(LoadImage(GET_IMAGE_PATH("snake-icon.png")));
    SetWindowMinSize(INIT_BOARD_W, INIT_BOARD_H);
    SetTargetFPS(60);

    Arena arena = ArenaCreateCustom(INIT_BOARD_TOTAL_ELEMENTS * sizeof(bool));
    Board board = {
        .cells = ArenaPushZero(&arena, INIT_BOARD_TOTAL_ELEMENTS),
        .height = INIT_BOARD_H,
        .width = INIT_BOARD_W
    };

    u32 stepMilliseconds = INIT_STEP_MILLISECONDS;
    f64 millisecondsSinceLastStep = 0.;
    BoardPrintData pData = getCellSize(board);

    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        millisecondsSinceLastStep += GetFrameTime() * 1000.;

        // INPUT SECTION
        if (IsWindowResized()) { pData = getCellSize(board); }

        // LOGIC SECTION
        if (millisecondsSinceLastStep - (f64)stepMilliseconds > 0) {
            millisecondsSinceLastStep -= (f64)stepMilliseconds;
            // stepBoard(board);
        }

        log_info("Screen [W=%d,H=%d]", GetScreenWidth(), GetScreenHeight());
        log_info("Board [W=%u,H=%u]", board.width, board.height);
        log_info("Size [CS=%u,OX=%u,OY=%u]", pData.cellSize, pData.offsetX, pData.offsetY);

        // DRAWING SECTION
        BeginDrawing();
        // ------------------------

        ClearBackground(DARKGRAY);

        drawBoard(board, pData);

        DrawFPS(10, 10);

        // ------------------------
        EndDrawing();
    }

    CloseWindow();

    return 0;
}

#pragma endregion
