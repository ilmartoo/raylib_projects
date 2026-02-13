#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "types/dynamic-array.h"
#include "input/input-handler.h"
#include "raylib/config.h"
#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "types/types.h"

#pragma region  // Config //

DYNAMIC_ARRAY_DEFINITION(u8, DynamicDirectionArray)

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

#define InvertDirection(dir) (dir ^ 0b10)

#define IsDirectionAxisY(dir) (dir & 0b1)
#define IsDirectionAxisX(dir) (!IsDirectionAxisY(dir))

#define BOARD_X    40
#define BOARD_Y    30
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

typedef struct {
    Camera2D camera;

    Cell board[BOARD_SIZE];  // List of board cells
    Pair playerPos;
    DynamicDirectionArray snake;  // Player locations

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

Cell* getCell(Data* data, Pair pos) { return &data->board[pos.x + pos.y * BOARD_X]; }

#pragma endregion

#pragma region  // Fruit //

Pair createFruit() { return (Pair){.x = random(FRUIT_TYPES), .y = random(FRUIT_VARIATIONS)}; }

void drawFruit(Texture2D spritesheet, u8 fruit, u8 variation, Vector2 pos) {
    if (fruit >= FRUIT_TYPES) { fruit %= FRUIT_TYPES; }
    if (variation >= FRUIT_VARIATIONS) { variation %= FRUIT_VARIATIONS; }
    DrawTextureRec(spritesheet, (Rectangle){fruit * FRUIT_SPRITE_SIZE, variation * FRUIT_SPRITE_SIZE, FRUIT_SPRITE_SIZE, FRUIT_SPRITE_SIZE}, pos, WHITE);
}

#pragma endregion

#pragma region  // Snake //

void drawSnake(Texture2D spritesheet, u8 type, u8 direction, Vector2 pos) {
    if (type >= SNAKE_TYPES) { type %= SNAKE_TYPES; }
    if (direction >= SNAKE_DIRECTIONS) { direction %= SNAKE_DIRECTIONS; }
    DrawTextureRec(spritesheet, (Rectangle){type * SNAKE_SPRITE_SIZE, direction * SNAKE_SPRITE_SIZE, SNAKE_SPRITE_SIZE, SNAKE_SPRITE_SIZE}, pos, WHITE);
}

Pair movePos(Pair pos, u8 direction) {
    switch (direction) {
        case SNAKE_UP: return (Pair){.x = pos.x, .y = (pos.y > 0 ? pos.y - 1 : BOARD_Y - 1)};
        case SNAKE_DOWN: return (Pair){.x = pos.x, .y = (pos.y < BOARD_Y - 1 ? pos.y + 1 : 0)};
        case SNAKE_LEFT: return (Pair){.x = (pos.x > 0 ? pos.x - 1 : BOARD_X - 1), .y = pos.y};
        case SNAKE_RIGHT: return (Pair){.x = (pos.x < BOARD_X - 1 ? pos.x + 1 : 0), .y = pos.y};
        default: return pos;
    }
}

Pair snakeDirection(SnakeDirection next, SnakeDirection current) {
    bool verticalNext = IsDirectionAxisY(next);
    bool verticalCurr = IsDirectionAxisY(current);

    if (verticalNext ^ verticalCurr) {
        bool isUp = (verticalNext && next == SNAKE_UP) || (verticalCurr && current == SNAKE_UP);
        bool isRight = (!verticalNext && next == SNAKE_RIGHT) || (!verticalCurr && current == SNAKE_RIGHT);

        return (Pair){.x = SNAKE_TURN, .y = isUp ? (isRight ? SNAKE_RIGHT : SNAKE_UP) : (isRight ? SNAKE_DOWN : SNAKE_LEFT)};
    } else {
        return (Pair){.x = SNAKE_BODY, .y = current};
    }
}

void snakeMove(Data* data, SnakeDirection direction) {
    Pair newPlayerPos = movePos(data->playerPos, direction);
    Cell* cell = getCell(data, newPlayerPos);

    printf("Old player pos: (%d %d)\n", data->playerPos.x, data->playerPos.y);
    printf("Direction: (%d)\n", direction);
    printf("New player pos: (%d %d)\n", newPlayerPos.x, newPlayerPos.y);
    printf("New snake cell: (%d)\n", cell->type);

    switch (cell->type) {
        // End game if crashed with own body
        case CELL_TYPE_SNAKE: {
            data->gameOver = true;
        }

        // Fruit eated + points
        case CELL_TYPE_FRUIT: {
            data->points += 1;

            cell->type = CELL_TYPE_SNAKE;
            cell->snake.type = SNAKE_HEAD;
            cell->snake.direction = direction;

            cell = getCell(data, data->playerPos);
            Pair snake = snakeDirection(direction, cell->snake.direction);
            cell->snake.type = snake.x;
            cell->snake.direction = snake.y;

            DynamicDirectionArrayShift(&data->snake, InvertDirection(direction));
        }

        // Basic movement
        default: {
            cell->type = CELL_TYPE_SNAKE;
            cell->snake.type = SNAKE_HEAD;
            cell->snake.direction = direction;

            Pair pos = data->playerPos;
            Pair snake = snakeDirection(direction, cell->snake.direction);
            cell->snake.type = snake.x;
            cell->snake.direction = snake.y;

            for (usize i = 1; i < data->snake.length - 1; ++i) { pos = movePos(pos, data->snake.elements[i]); }
            getCell(data, pos)->snake.type = SNAKE_TAIL;
            getCell(data, movePos(pos, data->snake.elements[data->snake.length - 1]))->type = CELL_TYPE_EMPTY;

            DynamicDirectionArrayShift(&data->snake, InvertDirection(direction));
            DynamicDirectionArrayPop(&data->snake);
        }
    }

    data->playerPos = newPlayerPos;
}

#pragma endregion

#pragma region  // Board //

void resetBoard(Data* data) {
    DynamicDirectionArrayClear(&data->snake);

    for (usize i = 0; i < BOARD_SIZE; ++i) { data->board[i] = (Cell){0}; }
    Pair pos = {.x = (BOARD_X / 2), .y = (BOARD_Y / 2)};
    Cell* cell;

    pos.y -= 1;

    cell = getCell(data, pos);
    cell->type = CELL_TYPE_FRUIT;
    Pair fruit = createFruit();
    cell->fruit.type = fruit.x;
    cell->fruit.variation = fruit.y;

    pos.y += 2;

    data->playerPos = pos;

    cell = getCell(data, pos);
    cell->type = CELL_TYPE_SNAKE;
    cell->snake.type = SNAKE_HEAD;
    cell->snake.direction = SNAKE_UP;

    pos.y += 1;
    DynamicDirectionArrayShift(&data->snake, SNAKE_DOWN);

    cell = getCell(data, pos);
    cell->type = CELL_TYPE_SNAKE;
    cell->snake.type = SNAKE_TAIL;
    cell->snake.direction = SNAKE_UP;

    printf("\n");
    printf("DYNAMIC ARRAY ELEMS: %u\n", data->snake.length);
    printf("DYNAMIC ARRAY SIZE: %u\n", data->snake.size);
    printf("\n");

    printf("\n");
    printf(">> [START DIRECTIONS\n");
    for_dynamic_array(u8, dir, data->snake) { printf(">> [ Direction: %hhu\n", dir); }
    printf(">> [END DIRECTIONS\n");
    printf("\n");
}

void drawCell(Vector2 pos, bool alterColor) { DrawRectangle(pos.x, pos.y, CELL_SIZE, CELL_SIZE, alterColor ? CELL_COLOR_1 : CELL_COLOR_2); }

#pragma endregion

#pragma region  // Logic //

#define SCREEN_PADDING 30
#define WORLD_SIZE_X   ((BOARD_X * CELL_SIZE) + (SCREEN_PADDING << 1))
#define WORLD_SIZE_Y   ((BOARD_Y * CELL_SIZE) + (SCREEN_PADDING << 1))

void manageGeneralInputs(Data* data) {
    if (GreedyInputHandlerGetValue(data->inputHandler, INPUT_TOGGLE_FULLSCREEN).b) { ToggleFullscreen(); }
}

void manageMovementInputs(Data* data) {
    InputResult* inputs = GreedyInputHandlerGetAllValues(data->inputHandler);

    data->deltaToMovement -= GetFrameTime();
    if (data->deltaToMovement <= 0) {
        data->deltaToMovement = SECONDS_FOR_SNAKE_MOVE - data->deltaToMovement;

        SnakeDirection nextSnakeDirection = data->snake.elements[0];

        // Check input and do not allow to go backwards
        if (inputs[INPUT_MOVE_DOWN].b && nextSnakeDirection != SNAKE_DOWN) {
            snakeMove(data, SNAKE_DOWN);
        } else if (inputs[INPUT_MOVE_LEFT].b && nextSnakeDirection != SNAKE_LEFT) {
            snakeMove(data, SNAKE_LEFT);
        } else if (inputs[INPUT_MOVE_UP].b && nextSnakeDirection != SNAKE_UP) {
            snakeMove(data, SNAKE_UP);
        } else if (inputs[INPUT_MOVE_RIGHT].b && nextSnakeDirection != SNAKE_RIGHT) {
            snakeMove(data, SNAKE_RIGHT);
        } else {
            printf("Invert snake direction %d\n", data->snake.elements[0]);
            snakeMove(data, InvertDirection(data->snake.elements[0]));
        }
    }
}

void generateFruit(Data* data) {
    data->deltaToNewFruit -= GetFrameTime();
    if (data->deltaToNewFruit <= 0) {
        data->deltaToNewFruit = SECONDS_FOR_NEW_FRUIT - data->deltaToNewFruit;
        Cell* cell;
        while ((cell = getCell(data, (Pair){.x = random(BOARD_X), .y = random(BOARD_Y)}))->type != CELL_TYPE_EMPTY) {}
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
            case CELL_TYPE_SNAKE: {
                drawSnake(data.snakeSpritesheet, cell.snake.type, cell.snake.direction, pos);
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

    DynamicDirectionArrayCreate(&data.snake);

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

        GreedyInputHandlerUpdate(&data.inputHandler);

        manageGeneralInputs(&data);

        if (!data.gameOver) {
            generateFruit(&data);
            manageMovementInputs(&data);
        }

        drawWorld(data);

        if (data.gameOver) {
            // BeginDrawing();
            // DrawText("GAME OVER", GetScreenWidth() / 2, GetScreenHeight() / 2, 24, WHITE);
            // EndDrawing();
            break;
        }
    }

    GreedyInputHandlerDelete(&data.inputHandler);

    DynamicDirectionArrayDelete(&data.snake);

    UnloadTexture(data.fruitSpritesheet);
    UnloadTexture(data.snakeSpritesheet);
}

#pragma endregion
