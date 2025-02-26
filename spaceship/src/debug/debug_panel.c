
#include <stdlib.h>

#include "arena.h"
#include "debug_panel.h"
#include "memory_utils.h"
#include "raylib.h"
#include "types.h"

#define DEBUG_PANEL_SECTION_FONT_SIZE 24
#define DEBUG_PANEL_ENTRY_FONT_SIZE   24
#define DEBUG_PANEL_FONT_SPACING      0

#define DEBUG_PANEL_BORDER_SIZE     2
#define DEBUG_PANEL_LINE_SIZE       1
#define DEBUG_PANEL_PADDING_X       6
#define DEBUG_PANEL_PADDING_Y       2
#define DEBUG_PANEL_SECTION_SPACING 4

DebugPanel *debug_panel_create(Color background_color, Font *section_font, Font *entry_font)
{
    DebugPanel *panel = reserve(DebugPanel);
    *panel = (DebugPanel){.arena = arena_create(),
                          .background_color = background_color,
                          .fonts = {.section = section_font, .entry = entry_font},
                          .sections = {NULL, NULL}};
    return panel;
}

void debug_panel_delete(DebugPanel *panel)
{
    areana_delete(&panel->arena);
    free(panel);
}

void debug_panel_add_section(DebugPanel *panel, const char *title)
{
    usize title_char_count = TextLength(title) + 1;

    DebugPanelSection *section = arena_push_type(&panel->arena, DebugPanelSection);
    *section = (DebugPanelSection){
        .title = title == NULL ? NULL : memory_copy(arena_push(&panel->arena, title_char_count), title, title_char_count),
        .text_measures = title == NULL
                             ? (Vector2){0}
                             : MeasureTextEx(*panel->fonts.section, title, DEBUG_PANEL_SECTION_FONT_SIZE, DEBUG_PANEL_FONT_SPACING),
        .entries = {NULL, NULL},
        .next = NULL};

    if (panel->sections.first == NULL) { panel->sections.first = panel->sections.last = section; }
    else { panel->sections.last = panel->sections.last->next = section; }
}

void debug_panel_add_entry(DebugPanel *panel, const char *text)
{
    DebugPanelSection *last_section = panel->sections.last;
    usize text_char_count = TextLength(text) + 1;

    DebugPanelEntry *entry = arena_push_type(&panel->arena, DebugPanelEntry);
    *entry =
        (DebugPanelEntry){.text = memory_copy(arena_push(&panel->arena, text_char_count), text, text_char_count),
                          .text_measures = MeasureTextEx(*panel->fonts.entry, text, DEBUG_PANEL_ENTRY_FONT_SIZE, DEBUG_PANEL_FONT_SPACING),
                          .next = NULL};

    if (last_section->entries.first == NULL) { last_section->entries.first = last_section->entries.last = entry; }
    else { last_section->entries.last = last_section->entries.last->next = entry; }
}

void draw_text(const char *text, f32 x, f32 y, float font_size, Font font) { DrawTextEx(font, text, (Vector2){x, y}, font_size, 0, WHITE); }

Vector2 debug_panel_measure(DebugPanel panel)
{
    Vector2 measures = {0};

    DebugPanelSection *section = panel.sections.first;
    DebugPanelEntry *entry = NULL;
    while (section != NULL)
    {
        Vector2 title_measure = section->text_measures;
        measures.x = max(measures.x, title_measure.x);
        measures.y += title_measure.y + DEBUG_PANEL_LINE_SIZE + DEBUG_PANEL_SECTION_SPACING;

        entry = section->entries.first;
        while (entry != NULL)
        {
            Vector2 text_measure = entry->text_measures;
            measures.x = max(measures.x, text_measure.x);
            measures.y += text_measure.y;

            entry = entry->next;
        }

        section = section->next;
    }

    measures.x += (DEBUG_PANEL_PADDING_X + DEBUG_PANEL_BORDER_SIZE) * 2;
    measures.y += (DEBUG_PANEL_PADDING_Y + DEBUG_PANEL_BORDER_SIZE) * 2;
    return measures;
}

void debug_panel_draw(DebugPanel panel, Vector2 screen_position)
{
    f32 x = screen_position.x, y = screen_position.y;

    Vector2 measures = debug_panel_measure(panel);

    DrawRectangle(screen_position.x, screen_position.y, measures.x, measures.y, Fade(panel.background_color, 0.25));
    DrawRectangleLinesEx((Rectangle){screen_position.x, screen_position.y, measures.x, measures.y}, DEBUG_PANEL_BORDER_SIZE,
                         panel.background_color);

    x += DEBUG_PANEL_PADDING_X + DEBUG_PANEL_BORDER_SIZE;
    y += DEBUG_PANEL_PADDING_Y + DEBUG_PANEL_BORDER_SIZE;

    DebugPanelSection *section = panel.sections.first;
    DebugPanelEntry *entry = NULL;
    while (section != NULL)
    {
        if (section->title != NULL)
        {
            draw_text(section->title, x, y, DEBUG_PANEL_SECTION_FONT_SIZE, *panel.fonts.section);
            y += section->text_measures.y;
        }

        DrawLineEx((Vector2){x, y}, (Vector2){x + section->text_measures.x, y}, DEBUG_PANEL_LINE_SIZE, WHITE);
        y += DEBUG_PANEL_LINE_SIZE;

        entry = section->entries.first;
        while (entry != NULL)
        {
            draw_text(entry->text, x, y, DEBUG_PANEL_ENTRY_FONT_SIZE, *panel.fonts.entry);
            y += entry->text_measures.y;

            entry = entry->next;
        }

        y += DEBUG_PANEL_SECTION_SPACING;
        section = section->next;
    }
}

void debug_panel_clean(DebugPanel *panel)
{
    panel->sections.first = NULL;
    panel->sections.last = NULL;
    arena_clear(&panel->arena);
}
