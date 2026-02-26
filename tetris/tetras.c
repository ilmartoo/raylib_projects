#include "raylib/raylib.h"
#include "raylib/raymath.h"
#include "types/types.h"
#include "types/game.h"

#define BOARD_X 10
#define BOARD_Y 30

typedef struct {
    Color color;
} Tetra;

typedef struct {
} Controlled;

typedef struct {
    Tetra board[BOARD_Y][BOARD_X];

} Game;

int main() {
    game_start();
    return 0;
}