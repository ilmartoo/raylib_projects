#pragma once
#ifndef GAME_H_
#define GAME_H_

/**
 * Initialization of the game and related data
 */
void game_init();
/**
 * Release of the game resources and related data
 */
void game_delete();

/**
 * Game loop exit condition
 * @return If the exit condition is met
 */
bool game_close();

/**
 * Game update on game loop iteration
 */
void game_update();
/**
 * Game draw on game loop iteration
 */
void game_draw();

/**
 * Execution of the game lifecycle
 */
void game_start() {
    game_init();
    while (!game_close()) {
        game_update();
        game_draw();
    }
    game_delete();
}

#endif  // GAME_H_