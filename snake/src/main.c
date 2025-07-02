#include <stdlib.h>

#include "input-handler.h"
#include "rayconfig.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"

// Fruit //

typedef enum {
    FRUIT_APPLE_RED = 0,
    FRUIT_APPLE_GREEN,
    FRUIT_APPLE_YELLOW,
    FRUIT_GRAPES_BLACK,
    FRUIT_GRAPES_GREEN,
    FRUIT_GRAPES_PURPLE,
    FRUIT_BANANA_GREEN,
    FRUIT_BANANA_YELLOW,
    FRUIT_BANANA_ORANGE,
    FRUIT_ORANGE,
    FRUIT_PEAR,
    FRUIT_PINEAPPLE,
    FRUIT_PEACH,
    FRUIT_FIG,
    FRUIT_CHERRY_RED,
    FRUIT_CHERRY_VIOLET,
    FRUIT_BLUEBERRY,
    FRUIT_LIME,
    FRUIT_LEMON,
    FRUIT_KIWIFRUIT,
    FRUIT_RAMBUTAN,
    FRUIT_DRAGON_FRUIT,
    FRUIT_COCONUT,
    FRUIT_TOMATO,
    FRUIT_FINGERED_CITRON,
    FRUIT_EGGPLANT,
    FRUIT_WATERMELON,
    FRUIT_STRAWBERRY,
    FRUIT_CHILI_GREEN,
    FRUIT_CHILI_YELLOW,
    FRUIT_CHILI_RED,
    FRUIT_PEPPER_GREEN,
    FRUIT_PEPPER_YELLOW,
    FRUIT_PEPPER_RED,
    FRUIT_PUMPKIN,
    FRUIT_RASPBERRY_RED,
    FRUIT_RASPBERRY_BLACK,
    FRUIT_AVOCADO,
} Fruit;

#define FRUIT_TYPES       38
#define FRUIT_VARIATIONS  6
#define FRUIT_SPRITE_SIZE 16

#define SECONDS_FOR_NEW_FRUIT 5

void drawFruit(Texture2D spritesheet, u8 fruit, u8 variation, Vector2 pos) {
    if (fruit >= FRUIT_TYPES) { fruit %= FRUIT_TYPES; }
    if (variation >= FRUIT_VARIATIONS) { variation %= FRUIT_VARIATIONS; }
    DrawTextureRec(spritesheet,
                   (Rectangle){fruit * FRUIT_SPRITE_SIZE, variation * FRUIT_SPRITE_SIZE, FRUIT_SPRITE_SIZE, FRUIT_SPRITE_SIZE},
                   pos,
                   WHITE);
}

// Snake //

#define SNAKE_TYPES       4
#define SNAKE_DIRECTIONS  4
#define SNAKE_SPRITE_SIZE 16

typedef enum {
    SNAKE_HEAD = 0,
    SNAKE_BODY,
    SNAKE_TAIL,
    SNAKE_TURN,
} SnakePart;

typedef enum {
    SNAKE_DOWN = 0,
    SNAKE_LEFT,
    SNAKE_UP,
    SNAKE_RIGHT,
} SnakeDirection;

void drawSnake(Texture2D spritesheet, u8 type, u8 direction, Vector2 pos) {
    if (type >= SNAKE_TYPES) { type %= SNAKE_TYPES; }
    if (direction >= SNAKE_DIRECTIONS) { direction %= SNAKE_DIRECTIONS; }
    DrawTextureRec(spritesheet,
                   (Rectangle){type * SNAKE_SPRITE_SIZE, direction * SNAKE_SPRITE_SIZE, SNAKE_SPRITE_SIZE, SNAKE_SPRITE_SIZE},
                   pos,
                   WHITE);
}

void snakeMove(SnakeDirection direction) { /*...*/ }

// Board //

#define BOARD_X    20
#define BOARD_Y    15
#define BOARD_SIZE (BOARD_X * BOARD_Y)

#define CELL_SIZE    16
#define CELL_COLOR_1 YELLOW
#define CELL_COLOR_2 LIME

typedef struct {
    union {
        u8 v[2];
        struct {
            u8 x;
            u8 y;
        };
    };
} U8Pair;

typedef struct {
    union {
        u16 v[2];
        struct {
            u16 x;
            u16 y;
        };
    };
} U16Pair;

typedef enum {
    CELL_TYPE_EMPTY,
    CELL_TYPE_SNAKE_START,
    CELL_TYPE_SNAKE_MIDDLE,
    CELL_TYPE_SNAKE_END,
    CELL_TYPE_FRUIT,
} CellType;

typedef struct {
    u8 type;
    union {
        struct {
            u8 type;
            u8 direction;
        } snake;
        struct {
            u8 type;
            u8 variation;
        } fruit;
    };
} Cell;

typedef struct {
    Camera2D camera;

    Cell board[BOARD_SIZE];  // List of board cells
    U8Pair player;           // Player head location

    double deltaToNewFruit;

    GreedyInputHandler inputHandler;

    Texture2D fruitSpritesheet;
    Texture2D snakeSpritesheet;
} Data;

void drawCell(Vector2 pos, bool alterColor) { DrawRectangle(pos.x, pos.y, CELL_SIZE, CELL_SIZE, alterColor ? CELL_COLOR_1 : CELL_COLOR_2); }

// Inputs //

typedef enum {
    INPUT_MOVE_DOWN = 0,
    INPUT_MOVE_LEFT,
    INPUT_MOVE_UP,
    INPUT_MOVE_RIGHT,
    INPUT_TOGGLE_FULLSCREEN,
} Inputs;

void manageInputs(Data *data) {
    InputResult *inputs = GreedyInputHandlerUpdate(&data->inputHandler).results;

    if (inputs[INPUT_TOGGLE_FULLSCREEN].bool_v) { ToggleFullscreen(); }

    if (inputs[INPUT_MOVE_DOWN].bool_v) {
        snakeMove(SNAKE_DOWN);
    } else if (inputs[INPUT_MOVE_LEFT].bool_v) {
        snakeMove(SNAKE_LEFT);
    } else if (inputs[INPUT_MOVE_UP].bool_v) {
        snakeMove(SNAKE_UP);
    } else if (inputs[INPUT_MOVE_RIGHT].bool_v) {
        snakeMove(SNAKE_RIGHT);
    }
}

// Logic //

void generateFruit(Data *data) {}

void drawWorld(Data data) {
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(data.camera);
    Vector2 pos;

    for (usize i = 0; i < BOARD_SIZE; ++i) {
        Cell cell = data.board[i];

        u8 x = i % BOARD_X;
        u8 y = i / BOARD_X;

        pos = (Vector2){x * CELL_SIZE, y * CELL_SIZE};

        drawCell(pos, (x + y) & 0b1);

        switch (cell.type) {
            case CELL_TYPE_SNAKE_START:
            case CELL_TYPE_SNAKE_MIDDLE:
            case CELL_TYPE_SNAKE_END: {
                drawSnake(data.snakeSpritesheet, cell.snake.type, cell.snake.direction, pos);
            } break;
            case CELL_TYPE_FRUIT: {
                drawFruit(data.fruitSpritesheet, cell.fruit.type, cell.fruit.variation, pos);
            } break;
        }
    }

    DrawRectangleLines(0, 0, BOARD_X * CELL_SIZE, BOARD_Y + CELL_SIZE, DARKGRAY);

    EndMode2D();

    DrawText("Fullscreen-compatible fixed camera", 10, 10, 20, WHITE);
    EndDrawing();
}

void recalculateCamera(Data *data) {
    u16 sw = GetScreenWidth();
    u16 sh = GetScreenHeight();

    data->camera.zoom = fminf((float)sw / (BOARD_X * CELL_SIZE), (float)sh / (BOARD_Y * CELL_SIZE));
    data->camera.offset = (Vector2){sw / 2, sh / 2};
}

void resetBoard(Data *data) {
    for (usize i = 0; i < BOARD_SIZE; ++i) { data->board[i] = (Cell){0}; }
    U8Pair ppos = {.x = (BOARD_X / 2), .y = (BOARD_Y / 2 - 1)};
    data->player = ppos;

    Cell *cell = data->board;

    cell += ppos.x + (ppos.y * BOARD_X);
    cell->type = CELL_TYPE_SNAKE_START;
    cell->snake.type = SNAKE_HEAD;
    cell->snake.direction = SNAKE_DOWN;

    cell += BOARD_X;
    cell->type = CELL_TYPE_SNAKE_END;
    cell->snake.type = SNAKE_TAIL;
    cell->snake.direction = SNAKE_DOWN;
}

i32 main() {
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Snake");
    SetExitKey(KEY_ESCAPE);
    SetTargetFPS(0);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Data data = {0};

    data.camera.target = (Vector2){BOARD_X * CELL_SIZE * 0.5f, BOARD_Y * CELL_SIZE * 0.5f};
    data.camera.rotation = 0;

    resetBoard(&data);

    data.deltaToNewFruit = SECONDS_FOR_NEW_FRUIT;

    data.inputHandler = GreedyInputHandlerCreate(1);
    GreedyInputHandlerMapSet(&data.inputHandler, INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE, 0, MAP_KEYBOARD_KEY_PRESSED(KEY_F11));

    data.fruitSpritesheet = LoadTexture("assets/fruits.png");
    data.snakeSpritesheet = LoadTexture("assets/snake.png");

    while (!WindowShouldClose()) {
        recalculateCamera(&data);

        drawWorld(data);
    }

    GreedyInputHandlerDelete(&data.inputHandler);

    UnloadTexture(data.fruitSpritesheet);
    UnloadTexture(data.snakeSpritesheet);

    return 0;
}