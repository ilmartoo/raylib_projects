#include <stdlib.h>
#include <time.h>

#include "dynamic-array.h"
#include "input-handler.h"
#include "rayconfig.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"

// Utils //

typedef struct {
    union {
        u8 v[2];
        struct {
            u8 x;
            u8 y;
        };
    };
} Pair;

#define FRUIT_TYPES       38
#define FRUIT_VARIATIONS  6
#define FRUIT_SPRITE_SIZE 16

#define SECONDS_FOR_NEW_FRUIT 5

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

#define SNAKE_TYPES       4
#define SNAKE_DIRECTIONS  4
#define SNAKE_SPRITE_SIZE 16

#define SECONDS_FOR_SNAKE_MOVE (0.75f)

typedef enum {
    SNAKE_HEAD = 0,
    SNAKE_BODY,
    SNAKE_TAIL,
    SNAKE_TURN,
} SnakePart;

typedef enum {
    SNAKE_UP = 0,
    SNAKE_RIGHT,
    SNAKE_DOWN,
    SNAKE_LEFT,
} SnakeDirection;

#define AreDirsDiffAxis(dir1, dir2) ((dir1 ^ dir2) & 0b1)
#define AreDirsSameAxis(dir1, dir2) (!AreDirsDiffAxis(dir1, dir2))

#define BOARD_X    20
#define BOARD_Y    15
#define BOARD_SIZE (BOARD_X * BOARD_Y)

#define CELL_SIZE    16
#define CELL_COLOR_1 YELLOW
#define CELL_COLOR_2 LIME

typedef enum {
    CELL_TYPE_EMPTY,
    CELL_TYPE_SNAKE,
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

DYNAMIC_ARRAY_DEFINITION(Pair, DynamicPairArray)

typedef struct {
    Camera2D camera;

    Cell board[BOARD_SIZE];   // List of board cells
    DynamicPairArray player;  // Player locations

    float deltaToNewFruit;
    float deltaToMovement;
    u16 points;
    bool gameOver;

    GreedyInputHandler inputHandler;

    Texture2D fruitSpritesheet;
    Texture2D snakeSpritesheet;
} Data;

typedef enum {
    INPUT_MOVE_DOWN = 0,
    INPUT_MOVE_LEFT,
    INPUT_MOVE_UP,
    INPUT_MOVE_RIGHT,
    INPUT_TOGGLE_FULLSCREEN,
} Inputs;

#define INPUT_MAPPINGS_SIZE 5

u8 random(u8 range) {
    if (range <= 0) return 0;
    u8 limit = (RAND_MAX / range) * range;
    u32 r;
    while ((r = rand()) >= limit) {}
    return r % range;
}

// Fruit //

Pair createFruit() { return (Pair){.x = random(FRUIT_TYPES), .y = random(FRUIT_VARIATIONS)}; }

void drawFruit(Texture2D spritesheet, u8 fruit, u8 variation, Vector2 pos) {
    if (fruit >= FRUIT_TYPES) { fruit %= FRUIT_TYPES; }
    if (variation >= FRUIT_VARIATIONS) { variation %= FRUIT_VARIATIONS; }
    DrawTextureRec(spritesheet,
                   (Rectangle){fruit * FRUIT_SPRITE_SIZE, variation * FRUIT_SPRITE_SIZE, FRUIT_SPRITE_SIZE, FRUIT_SPRITE_SIZE},
                   pos,
                   WHITE);
}

// Snake //

void drawSnake(Texture2D spritesheet, u8 type, u8 direction, Vector2 pos) {
    if (type >= SNAKE_TYPES) { type %= SNAKE_TYPES; }
    if (direction >= SNAKE_DIRECTIONS) { direction %= SNAKE_DIRECTIONS; }
    DrawTextureRec(spritesheet,
                   (Rectangle){type * SNAKE_SPRITE_SIZE, direction * SNAKE_SPRITE_SIZE, SNAKE_SPRITE_SIZE, SNAKE_SPRITE_SIZE},
                   pos,
                   WHITE);
}

Pair movePos(Pair pos, SnakeDirection direction) {
    switch (direction) {
        case SNAKE_UP: return (Pair){.x = pos.x, .y = (pos.y > 0 ? pos.y - 1 : BOARD_Y - 1)};
        case SNAKE_DOWN: return (Pair){.x = pos.x, .y = (pos.y < BOARD_Y - 1 ? pos.y + 1 : 0)};
        case SNAKE_LEFT: return (Pair){.x = (pos.x > 0 ? pos.x - 1 : BOARD_X - 1), .y = pos.y};
        case SNAKE_RIGHT: return (Pair){.x = (pos.x < BOARD_X - 1 ? pos.x + 1 : 0), .y = pos.y};
    }
}

void snakeMove(Data *data, SnakeDirection direction) {
    Pair playerPos = data->player.elements[0];
    Pair pos = movePos(playerPos, direction);

    Cell *cell = boardCell(data, pos.x, pos.y);

    switch (cell->type) {
        // End game if crashed with own body
        case CELL_TYPE_SNAKE: {
            data->gameOver = true;
            return;
        }

        // Fruit eated + points
        case CELL_TYPE_FRUIT: {
            data->points += 1;
            boardCell(data, playerPos.x, playerPos.y)->snake.type = SNAKE_BODY;
            *boardCell(data, pos.x, pos.y) = (Cell){.type = CELL_TYPE_SNAKE, .snake = {.type = SNAKE_HEAD, .direction = direction}};
            DynamicPairArrayShift(&data->player, pos);
            return;
        }

        // Basic movement
        default: {
        }
    }

    // Straight line
    if (InvertDirection(direction) == cell->snake.from) {
        Cell *oldHead = boardCell(data, data->player.x, data->player.y);
        *cell = *oldHead;
        oldHead->type = CELL_TYPE_SNAKE;
        oldHead->snake.type = SNAKE_BODY;
    }
    // A turn
    else {
        Cell *oldHead = boardCell(data, data->player.x, data->player.y);
        *cell = *oldHead;
        oldHead->type = CELL_TYPE_SNAKE;
        oldHead->snake.type = SNAKE_BODY;
    }
    data->player = pos;

    // transform player location into dynamic array of positions

    while (cell->type != CELL_TYPE_SNAKE_END) { *nextCell = *cell; }

    *cell = (Cell){CELL_TYPE_EMPTY, {0}};  // Set empty the last cell
}

// Board //

Cell *boardCell(Data *data, u8 x, u8 y) { return &data->board[x + y * BOARD_X]; }

void resetBoard(Data *data) {
    DynamicPairArrayClear(&data->player);

    for (usize i = 0; i < BOARD_SIZE; ++i) { data->board[i] = (Cell){0}; }
    Pair pos = {.x = (BOARD_X >> 1), .y = (BOARD_Y >> 1)};

    // Initialize array of player positions

    Cell *cell;

    pos.y -= 1;

    cell = boardCell(data, pos.x, pos.y);
    cell->type = CELL_TYPE_FRUIT;
    Pair fruit = createFruit();
    cell->fruit.type = fruit.x;
    cell->fruit.variation = fruit.y;

    pos.y += 2;

    DynamicPairArrayShift(&data->player, pos);

    cell = boardCell(data, pos.x, pos.y);
    cell->type = CELL_TYPE_SNAKE_START;
    cell->snake.type = SNAKE_HEAD;
    cell->snake.direction = SNAKE_UP;

    pos.y += 1;

    DynamicPairArrayShift(&data->player, pos);

    cell = boardCell(data, pos.x, pos.y);
    cell->type = CELL_TYPE_SNAKE_END;
    cell->snake.type = SNAKE_TAIL;
    cell->snake.direction = SNAKE_UP;
}

void drawCell(Vector2 pos, bool alterColor) { DrawRectangle(pos.x, pos.y, CELL_SIZE, CELL_SIZE, alterColor ? CELL_COLOR_1 : CELL_COLOR_2); }

// Logic //

#define SCREEN_PADDING 10
#define WORLD_SIZE_X   ((BOARD_X * CELL_SIZE) + (SCREEN_PADDING << 1))
#define WORLD_SIZE_Y   ((BOARD_Y * CELL_SIZE) + (SCREEN_PADDING << 1))

void manageInputs(Data *data) {
    InputResult *inputs = GreedyInputHandlerUpdate(&data->inputHandler).results;

    if (inputs[INPUT_TOGGLE_FULLSCREEN].bool_v) { ToggleFullscreen(); }

    data->deltaToMovement -= GetFrameTime();
    if (data->deltaToMovement <= 0) {
        data->deltaToMovement = SECONDS_FOR_SNAKE_MOVE - data->deltaToMovement;

        SnakeDirection fromDirection = boardCell(data, data->player.x, data->player.y)->snake.from;

        // Check input and do not allow to go backwards
        if (inputs[INPUT_MOVE_DOWN].bool_v && fromDirection != SNAKE_DOWN) {
            snakeMove(data, SNAKE_DOWN);
        } else if (inputs[INPUT_MOVE_LEFT].bool_v && fromDirection != SNAKE_LEFT) {
            snakeMove(data, SNAKE_LEFT);
        } else if (inputs[INPUT_MOVE_UP].bool_v && fromDirection != SNAKE_UP) {
            snakeMove(data, SNAKE_UP);
        } else if (inputs[INPUT_MOVE_RIGHT].bool_v && fromDirection != SNAKE_RIGHT) {
            snakeMove(data, SNAKE_RIGHT);
        } else {
            snakeMove(data, InvertDirection(boardCell(data, data->player.x, data->player.y)->snake.from));
        }
    }
}

void generateFruit(Data *data) {
    data->deltaToNewFruit -= GetFrameTime();
    if (data->deltaToNewFruit <= 0) {
        data->deltaToNewFruit = SECONDS_FOR_NEW_FRUIT - data->deltaToNewFruit;
        Cell *cell;
        while ((cell = boardCell(data, random(BOARD_X), random(BOARD_Y)))->type != CELL_TYPE_EMPTY) {}
        Pair fruit = createFruit();
        cell->type = CELL_TYPE_FRUIT;
        cell->fruit.type = fruit.x;
        cell->fruit.variation = fruit.y;
    }
}

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
                drawSnake(data.snakeSpritesheet, cell.snake.type, cell.snake.from, pos);
            } break;
            case CELL_TYPE_FRUIT: {
                drawFruit(data.fruitSpritesheet, cell.fruit.type, cell.fruit.variation, pos);
            } break;
        }
    }
    EndMode2D();

    DrawText("Fullscreen-compatible fixed camera", 10, 10, 20, WHITE);
    EndDrawing();
}

void recalculateCamera(Data *data) {
    u16 sw = GetScreenWidth();
    u16 sh = GetScreenHeight();

    data->camera.zoom = fminf((float)sw / WORLD_SIZE_X, (float)sh / WORLD_SIZE_Y);
    data->camera.offset = (Vector2){sw / 2, sh / 2};
}

i32 main() {
    InitWindow(GetMonitorWidth(GetCurrentMonitor()), GetMonitorHeight(GetCurrentMonitor()), "Snake");
    SetExitKey(KEY_ESCAPE);
    SetTargetFPS(0);
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    Data data = {0};

    data.camera.target = (Vector2){(BOARD_X * CELL_SIZE) >> 1, (BOARD_Y * CELL_SIZE) >> 1};
    data.camera.rotation = 0;

    DynamicPairArrayCreate(&data.player);

    resetBoard(&data);

    data.deltaToNewFruit = SECONDS_FOR_NEW_FRUIT;

    data.inputHandler = GreedyInputHandlerCreate(INPUT_MAPPINGS_SIZE);
    InputMap inputMaps[2][INPUT_MAPPINGS_SIZE] = {
        {
            MAP_KEYBOARD_KEY_DOWN(KEY_DOWN),
            MAP_KEYBOARD_KEY_DOWN(KEY_LEFT),
            MAP_KEYBOARD_KEY_DOWN(KEY_UP),
            MAP_KEYBOARD_KEY_DOWN(KEY_RIGHT),
            MAP_KEYBOARD_KEY_DOWN(KEY_F11),
        },
        {
            MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_LEFT_Y, AXIS_RANGE_NEGATIVE),
            MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_LEFT_X, AXIS_RANGE_POSITIVE),
            MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_LEFT_Y, AXIS_RANGE_POSITIVE),
            MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_LEFT_X, AXIS_RANGE_NEGATIVE),
            MAP_NONE,
        },
    };
    GreedyInputHandlerMappingsSet(&data.inputHandler, inputMaps[0], inputMaps[1]);

    data.fruitSpritesheet = LoadTexture("assets/fruits.png");
    data.snakeSpritesheet = LoadTexture("assets/snake.png");

    srand(time(NULL));

    while (!WindowShouldClose()) {
        recalculateCamera(&data);
        if (!data.gameOver) {
            generateFruit(&data);
            manageInputs(&data);
        } else {
        }
        drawWorld(data);
    }

    GreedyInputHandlerDelete(&data.inputHandler);

    UnloadTexture(data.fruitSpritesheet);
    UnloadTexture(data.snakeSpritesheet);

    return 0;
}