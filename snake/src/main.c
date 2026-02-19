#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types/list.h"
#include "input/input-handler.h"
#include "raylib/config.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "types/types.h"

#pragma region  // Config //

#define FRUIT_TYPES       38
#define FRUIT_VARIATIONS  6
#define FRUIT_SPRITE_SIZE 16

#define SNAKE_TYPES       4
#define SNAKE_DIRECTIONS  4
#define SNAKE_SPRITE_SIZE 16

#define BOARD_X 15
#define BOARD_Y 14

#define SECONDS_FOR_SNAKE_MOVE 0.15F
#define SECONDS_FOR_NEW_FRUIT  5

#define CELL_SIZE    16
#define CELL_COLOR_1 ((Color){40, 40, 40, 255})
#define CELL_COLOR_2 ((Color){30, 30, 30, 255})

#define SNAKE_SIZE              10
#define SNAKE_BORDER_SIZE       12
#define SNAKE_COLOR             LIME
#define SNAKE_BORDER_COLOR      GREEN
#define SNAKE_HEAD_BORDER_COLOR RED
#define SNAKE_TAIL_BORDER_COLOR YELLOW

#define FRUIT_SIZE 12

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
} FruitType;

typedef enum {
    MOVE_UP = 0,
    MOVE_RIGHT,
    MOVE_DOWN,
    MOVE_LEFT,
} Direction;

typedef enum {
    CELL_TYPE_EMPTY,
    CELL_TYPE_SNAKE,
    CELL_TYPE_FRUIT,
} CellType;

typedef struct {
    bool head;
    bool tail;
} SnakeCell;

typedef struct {
    FruitType type;
    u8 variation;
} FruitCell;

typedef struct {
    CellType type;
    union {
        SnakeCell snake;
        FruitCell fruit;
    };
} Cell;

typedef struct {
    u8 x;
    u8 y;
} U8Pair;

typedef List(Cell*) ListCellRef;

typedef struct {
    Camera2D camera;

    Cell board[BOARD_X][BOARD_Y];  // List of board cells

    struct {
        U8Pair player;
        ListCellRef snake;
    } position;

    struct {
        Direction intent;
        Direction last;
    } movement;

    struct {
        float fruit;
        float movement;
    } deltas;

    struct {
        u16 fruits;
        u16 points;
        bool gameOver;
    } state;

    struct {
        GreedyInputHandler handler;
    } input;

    struct {
        Texture2D fruit;
        // Texture2D snake;
    } spritesheets;
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

Cell* cellAt(Data* data, U8Pair pos) { return &data->board[pos.x][pos.y]; }

#pragma endregion

#pragma region  // Fruit //

U8Pair createFruit() { return (U8Pair){.x = random(FRUIT_TYPES), .y = random(FRUIT_VARIATIONS)}; }

void drawFruit(Texture2D spritesheet, u8 fruit, u8 variation, Vector2 pos) {
    if (fruit >= FRUIT_TYPES) { fruit %= FRUIT_TYPES; }
    if (variation >= FRUIT_VARIATIONS) { variation %= FRUIT_VARIATIONS; }

    u8 cellOffset = (CELL_SIZE - FRUIT_SIZE) / 2;
    Vector2 drawPos = Vector2AddValue(pos, cellOffset);

    DrawTextureRec(spritesheet, (Rectangle){fruit * FRUIT_SPRITE_SIZE, variation * FRUIT_SPRITE_SIZE, FRUIT_SIZE, FRUIT_SIZE}, drawPos, WHITE);
}

void generateFruit(Data* data) {
    data->deltas.fruit = SECONDS_FOR_NEW_FRUIT;
    Cell* cell;
    do { cell = cellAt(data, (U8Pair){.x = random(BOARD_X), .y = random(BOARD_Y)}); } while (cell->type != CELL_TYPE_EMPTY);
    U8Pair fruit = createFruit();
    cell->type = CELL_TYPE_FRUIT;
    cell->fruit.type = fruit.x;
    cell->fruit.variation = fruit.y;

    ++data->state.fruits;
}

void checkGenerateFruit(Data* data) {
    data->deltas.fruit -= GetFrameTime();
    if (data->deltas.fruit <= 0) {
        data->deltas.fruit = SECONDS_FOR_NEW_FRUIT + data->deltas.fruit;
        generateFruit(data);
    }
}

#pragma endregion

#pragma region  // Snake //

void drawSnake(Vector2 pos, bool head, bool tail) {
    u8 borderOffset = (CELL_SIZE - SNAKE_BORDER_SIZE) / 2;
    u8 cellOffset = (CELL_SIZE - SNAKE_SIZE) / 2;

    Color borderColor = head ? SNAKE_HEAD_BORDER_COLOR : tail ? SNAKE_TAIL_BORDER_COLOR : SNAKE_BORDER_COLOR;

    DrawRectangle(pos.x + borderOffset, pos.y + borderOffset, SNAKE_BORDER_SIZE, SNAKE_BORDER_SIZE, borderColor);
    DrawRectangle(pos.x + cellOffset, pos.y + cellOffset, SNAKE_SIZE, SNAKE_SIZE, SNAKE_COLOR);
}

U8Pair movePos(U8Pair pos, Direction direction) {
    switch (direction) {
        case MOVE_UP: return (U8Pair){.x = pos.x, .y = (pos.y > 0 ? pos.y - 1 : BOARD_Y - 1)};
        case MOVE_DOWN: return (U8Pair){.x = pos.x, .y = (pos.y + 1) % BOARD_Y};
        case MOVE_LEFT: return (U8Pair){.x = (pos.x > 0 ? pos.x - 1 : BOARD_X - 1), .y = pos.y};
        case MOVE_RIGHT: return (U8Pair){.x = (pos.x + 1) % BOARD_X, .y = pos.y};
        default: return pos;
    }
}

void snakeMove(Data* data, Direction direction) {
    U8Pair newPlayerPos = movePos(data->position.player, direction);
    Cell* cell = cellAt(data, newPlayerPos);

    // End game if crashed with own body
    if (cell->type == CELL_TYPE_SNAKE) {
        data->state.gameOver = true;
    } else {
        ListCellRef* list = &(data->position.snake);

        // Fruit eated > add points and generate fruit
        if (cell->type == CELL_TYPE_FRUIT) {
            data->state.points += 1;
            --data->state.fruits;

            if (data->state.fruits == 0) { generateFruit(data); }
        }
        // Basic movement > remove last snake cell
        else {
            (*list_item_last(list))->type = CELL_TYPE_EMPTY;
            list_pop(list);

            (*list_item_last(list))->snake.tail = true;
        }

        // Update new cell as new head cell
        (*list_item_first(list))->snake.head = false;

        *cell = (Cell){.type = CELL_TYPE_SNAKE, .snake = {.head = true, .tail = false}};
        list_prepend(list, cell);
    }

    data->position.player = newPlayerPos;
}

#pragma endregion

#pragma region  // Board //

void resetBoard(Data* data) {
    for (usize ix = 0; ix < BOARD_X; ++ix) {
        for (usize iy = 0; iy < BOARD_Y; ++iy) { data->board[ix][iy] = (Cell){.type = CELL_TYPE_EMPTY}; }
    }

    U8Pair pos = {.x = (BOARD_X / 2), .y = (BOARD_Y / 2)};

    ListCellRef* listSnake = &data->position.snake;
    list_clear(listSnake);

    // Head
    data->position.player = pos;
    Cell* cell = cellAt(data, pos);
    *cell = (Cell){.type = CELL_TYPE_SNAKE, .snake = {.head = true, .tail = false}};
    list_push(listSnake, cell);

    pos.y += 1;

    // Tail
    cell = cellAt(data, pos);
    *cell = (Cell){.type = CELL_TYPE_SNAKE, .snake = {.head = false, .tail = true}};
    list_push(listSnake, cell);

    generateFruit(data);

    data->movement.intent = MOVE_UP;
    data->movement.last = MOVE_UP;
}

void drawCell(Vector2 pos, bool alterColor) { DrawRectangle(pos.x, pos.y, CELL_SIZE, CELL_SIZE, alterColor ? CELL_COLOR_1 : CELL_COLOR_2); }

#pragma endregion

#pragma region  // Logic //

#define SCREEN_PADDING 30
#define WORLD_SIZE_X   ((BOARD_X * CELL_SIZE) + (SCREEN_PADDING << 1))
#define WORLD_SIZE_Y   ((BOARD_Y * CELL_SIZE) + (SCREEN_PADDING << 1))

void manageGeneralInputs(Data* data) {
    if (GreedyInputHandlerGetValue(data->input.handler, INPUT_TOGGLE_FULLSCREEN).b) { ToggleFullscreen(); }
}

void manageMovementInputs(Data* data) {
    InputResult* inputs = GreedyInputHandlerGetAllValues(data->input.handler);

    data->deltas.movement -= GetFrameTime();

    if (inputs[INPUT_MOVE_DOWN].b && data->movement.last != MOVE_UP) {
        data->movement.intent = MOVE_DOWN;
    } else if (inputs[INPUT_MOVE_LEFT].b && data->movement.last != MOVE_RIGHT) {
        data->movement.intent = MOVE_LEFT;
    } else if (inputs[INPUT_MOVE_UP].b && data->movement.last != MOVE_DOWN) {
        data->movement.intent = MOVE_UP;
    } else if (inputs[INPUT_MOVE_RIGHT].b && data->movement.last != MOVE_LEFT) {
        data->movement.intent = MOVE_RIGHT;
    }

    if (data->deltas.movement <= 0) {
        data->deltas.movement = SECONDS_FOR_SNAKE_MOVE + data->deltas.movement;
        snakeMove(data, data->movement.intent);
        data->movement.last = data->movement.intent;
    }
}

void drawWorld(Data data) {
    BeginDrawing();
    ClearBackground(BLACK);

    BeginMode2D(data.camera);
    Vector2 pos;

    for (usize ix = 0; ix < BOARD_X; ++ix) {
        for (usize iy = 0; iy < BOARD_Y; ++iy) {
            Cell cell = data.board[ix][iy];

            pos = (Vector2){ix * CELL_SIZE, iy * CELL_SIZE};

            drawCell(pos, (ix + iy) & 0b1);

            switch (cell.type) {
                case CELL_TYPE_SNAKE: {
                    drawSnake(pos, cell.snake.head, cell.snake.tail);
                } break;
                case CELL_TYPE_FRUIT: {
                    drawFruit(data.spritesheets.fruit, cell.fruit.type, cell.fruit.variation, pos);
                } break;
                default: break;
            }
        }
    }
    EndMode2D();

    DrawText("Fullscreen-compatible fixed camera", 10, 10, 20, WHITE);
    EndDrawing();
}

void recalculateCamera(Data* data) {
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

    data.camera.target = (Vector2){(BOARD_X * CELL_SIZE) / 2, (BOARD_Y * CELL_SIZE) / 2};
    data.camera.rotation = 0;

    resetBoard(&data);

    data.deltas.fruit = SECONDS_FOR_NEW_FRUIT;

    data.input.handler = GreedyInputHandlerCreate(INPUT_MAPPINGS_SIZE);
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
    GreedyInputHandlerMappingsSet(&data.input.handler, inputMaps[0], inputMaps[1]);

    data.spritesheets.fruit = LoadTexture("assets/fruits.png");
    // data.snakeSpritesheet = LoadTexture("assets/snake.png");

    srand(time(NULL));

    while (!WindowShouldClose()) {
        recalculateCamera(&data);

        GreedyInputHandlerUpdate(&data.input.handler);

        manageGeneralInputs(&data);

        if (!data.state.gameOver) {
            checkGenerateFruit(&data);
            manageMovementInputs(&data);
        }

        drawWorld(data);

        if (data.state.gameOver) {
            // BeginDrawing();
            // DrawText("GAME OVER", GetScreenWidth() / 2, GetScreenHeight() / 2, 24, WHITE);
            // EndDrawing();
            break;
        }
    }

    GreedyInputHandlerDelete(&data.input.handler);

    list_delete(&data.position.snake);

    UnloadTexture(data.spritesheets.fruit);
    // UnloadTexture(data.spritesheets.snake);
}

#pragma endregion
