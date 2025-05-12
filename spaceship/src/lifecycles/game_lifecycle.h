#pragma once
#ifndef ENGINE_H
#define ENGINE_H

#include "types.h"

/**
 * Initialize of the game.
 */
void GameInitialize(void);
/**
 * Loop where all the game logic is executed.
 */
void GameLoop(void);
/**
 * Check if the game should close
 */
bool GameShouldClose(void);
/**
 * Calculations that need to be done every frame.
 */
void GameFrame(void);
/**
 * Object drawing.
 */
void GameDraw(void);
/**
 * Cleaning and closure of the game memory.
 */
void GameClear(void);

#endif // ENGINE_H