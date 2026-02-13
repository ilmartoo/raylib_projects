#pragma once
#ifndef GAME_DEBUG_H
#define GAME_DEBUG_H

#ifdef DEBUG

#include "debug/debug_panel.h"

#define DEBUG_PANEL_TIMINGS_POSITION  ((Vector2){20, 20})
#define DEBUG_PANEL_ENTITIES_POSITION ((Vector2){20, 100})
#define DEBUG_PANEL_INPUTS_POSITION   ((Vector2){20, 330})

extern DebugPanel* timings_panel;
extern DebugPanel* entities_panel;
extern DebugPanel* inputs_panel;

/**
 * Debug panels initialization
 */
void GameDebugInitialize(void);
/**
 * Debug input
 */
void GameDebugInput(void);
/**
 * Debug panels update
 */
void GameDebugUpdate(void);
/**
 * Debug panels draw
 */
void GameDebugDraw(void);
/**
 * Debug panels clear
 */
void GameDebugClear(void);

#endif  // DEBUG

#endif  // GAME_DEBUG_H
