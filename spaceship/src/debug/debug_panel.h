#pragma once
#ifndef __SOURCE_DEBUG_PANEL_H_
#define __SOURCE_DEBUG_PANEL_H_

#include "arena.h"
#include "raylib.h"

typedef struct DebugPanelEntry
{
    const char *text;
    Vector2 text_measures;
    /* ... */
    struct DebugPanelEntry *next;
} DebugPanelEntry;

typedef struct DebugPanelSection
{
    const char *title;
    Vector2 text_measures;
    /* ... */
    struct
    {
        DebugPanelEntry *first;
        DebugPanelEntry *last;
    } entries;
    struct DebugPanelSection *next;
} DebugPanelSection;

typedef struct DebugPanel
{
    Arena arena;
    Color background_color;
    struct
    {
        Font section;
        Font entry;
    } fonts;
    /* ... */
    struct
    {
        DebugPanelSection *first;
        DebugPanelSection *last;
    } sections;
} DebugPanel;

/**
 * Creates a debug panel.
 * @param background_color Panel background color.
 * @param section_font Font for the section title.
 * @param entry_font Font for the entry text.
 * @returns A new debug panel.
 */
DebugPanel *debug_panel_create(Color background_color, Font section_font, Font entry_font);
/**
 * Deletes a debug panel.
 * @param panel Panel to delete.
 */
void debug_panel_delete(DebugPanel *panel);

/**
 * Appends a section to a debug panel.
 * @param panel Panel to use.
 * @param title Title of the section. Can be null.
 */
void debug_panel_add_section(DebugPanel *panel, const char *title);
/**
 * Appends an entry to the last section of a debug panel.
 * @param panel Panel to use.
 * @param text Text of the entry.
 */
void debug_panel_add_entry(DebugPanel *panel, const char *text);

/**
 * Cleans a debug panel for a new frame.
 * @param panel Panel to clean.
 */
void debug_panel_clean(DebugPanel *panel);
/**
 * Measures a debug panel.
 * @param panel Panel to measure.
 */
Vector2 debug_panel_measure(DebugPanel panel);

/**
 * Draws a debug panel.
 * @param panel Panel to draw.
 * @param screen_position Screen position where to draw the panel.
 */
void debug_panel_draw(DebugPanel panel, Vector2 screen_position);

#endif // __SOURCE_DEBUG_PANEL_H_