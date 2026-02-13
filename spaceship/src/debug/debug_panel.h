#pragma once
#ifndef DEBUG_PANEL_H
#define DEBUG_PANEL_H

#ifdef DEBUG

#include "types/arena.h"
#include "raylib/raylib.h"
#include "types/types.h"

typedef struct DebugPanel {
    Arena arena;
    Color background_color;
    Font font;
    u32 titles;
    u32 entries;
    Vector2 content_size;
} DebugPanel;

#define DEBUG_PANEL_TITLE_FONT_SIZE 20
#define DEBUG_PANEL_ENTRY_FONT_SIZE 15
#define DEBUG_PANEL_FONT_SPACING    0

#define DEBUG_PANEL_BORDER_SIZE   2
#define DEBUG_PANEL_PADDING_X     6
#define DEBUG_PANEL_PADDING_Y     2
#define DEBUG_PANEL_TITLE_SPACING 4

/**
 * Creates a debug panel.
 * @param background_color Panel background color.
 * @param section_font Font for the section title.
 * @param entry_font Font for the entry text.
 * @returns A new debug panel.
 */
DebugPanel* DebugPanelCreate(Color background_color, Font font);
/**
 * Deletes a debug panel.
 * @param panel Panel to delete.
 */
void DebugPanelDelete(DebugPanel* panel);

/**
 * Appends a section to a debug panel.
 * @param panel Panel to use.
 * @param title Title of the section. Can be null.
 */
void DebugPanelAddTitle(DebugPanel* panel, const char* title);
/**
 * Appends an entry to the last section of a debug panel.
 * @param panel Panel to use.
 * @param text Text of the entry. Can be null.
 */
void DebugPanelAddEntry(DebugPanel* panel, const char* text);

/**
 * Cleans a debug panel for a new frame.
 * @param panel Panel to clean.
 */
void DebugPanelClean(DebugPanel* panel);
/**
 * Measures a debug panel.
 * @param panel Panel to measure.
 */
Vector2 DebugPanelMeasures(DebugPanel panel);

/**
 * Draws a debug panel.
 * @param panel Panel to draw.
 * @param screen_position Screen position where to draw the panel.
 */
void DebugPanelDraw(DebugPanel panel, Vector2 screen_position);

#endif  // DEBUG

#endif  // DEBUG_PANEL_H
