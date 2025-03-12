
#include <stdlib.h>

#include "arena.h"
#include "debug_panel.h"
#include "memory_utils.h"
#include "rayheader.h"

DebugPanel *DebugPanelCreate(Color background_color, Font title_font, Font entry_font)
{
    DebugPanel *panel = reserve(DebugPanel);
    *panel = (DebugPanel){.arena = ArenaCreate(),
                          .background_color = background_color,
                          .font_title = title_font,
                          .font_entry = entry_font,
                          .titles = 0,
                          .entries = 0,
                          .content_size = {0}};
    return panel;
}

void DebugPanelDelete(DebugPanel *panel)
{
    ArenaDelete(&panel->arena);
    free(panel);
}

void DebugPanelAddTitle(DebugPanel *panel, const char *title)
{
    ++panel->titles;

    usize title_size = TextLength(title) + 1;

    *ArenaPushType(panel->arena, bool) = true;            // Is a title
    *ArenaPushTypeZero(panel->arena, usize) = title_size; // Text size
    f32 *height = ArenaPushTypeZero(panel->arena, f32);
    char *title_mem = ArenaPushArrayZero(panel->arena, char, title_size);

    if (title != NULL)
    {
        memory_copy(title_mem, title, title_size);
        Vector2 title_measures = MeasureTextEx(panel->font_title, title, DEBUG_PANEL_TITLE_FONT_SIZE, DEBUG_PANEL_FONT_SPACING);
        panel->content_size.x = max(panel->content_size.x, title_measures.x);
        *height = title_measures.y;
    }
}

void DebugPanelAddEntry(DebugPanel *panel, const char *text)
{
    ++panel->entries;

    usize text_size = TextLength(text) + 1;

    *ArenaPushType(panel->arena, bool) = false;          // Is an entry
    *ArenaPushTypeZero(panel->arena, usize) = text_size; // Text size
    f32 *height = ArenaPushTypeZero(panel->arena, f32);
    char *text_mem = ArenaPushArrayZero(panel->arena, char, text_size);

    if (text != NULL)
    {
        memory_copy(text_mem, text, text_size);
        Vector2 text_measures = MeasureTextEx(panel->font_entry, text, DEBUG_PANEL_ENTRY_FONT_SIZE, DEBUG_PANEL_FONT_SPACING);
        panel->content_size.x = max(panel->content_size.x, text_measures.x);
        panel->content_size.y += *height = text_measures.y;
    }
}

void draw_text(const char *text, f32 x, f32 y, float font_size, Font font) { DrawTextEx(font, text, (Vector2){x, y}, font_size, 0, WHITE); }

Vector2 DebugPanelMeasures(DebugPanel panel)
{
    Vector2 measures = panel.content_size;
    measures.y += panel.titles * DEBUG_PANEL_TITLE_SPACING;
    measures.x += (DEBUG_PANEL_PADDING_X + DEBUG_PANEL_BORDER_SIZE) * 2;
    measures.y += (DEBUG_PANEL_PADDING_Y + DEBUG_PANEL_BORDER_SIZE) * 2;
    return measures;
}

void DebugPanelDraw(DebugPanel panel, Vector2 screen_position)
{
    f32 x = screen_position.x, y = screen_position.y;

    Vector2 measures = DebugPanelMeasures(panel);

    DrawRectangle(screen_position.x, screen_position.y, measures.x, measures.y, Fade(panel.background_color, 0.25));
    DrawRectangleLinesEx(
        (Rectangle){screen_position.x, screen_position.y, measures.x, measures.y}, DEBUG_PANEL_BORDER_SIZE, panel.background_color);

    x += DEBUG_PANEL_PADDING_X + DEBUG_PANEL_BORDER_SIZE;
    y += DEBUG_PANEL_PADDING_Y + DEBUG_PANEL_BORDER_SIZE;

    u32 remaining_titles = panel.titles;
    u32 remaining_entries = panel.entries;
    void *ptr = panel.arena.memory;

    while (remaining_titles > 0 || remaining_entries > 0)
    {
        bool is_title = *(bool *)ptr;
        ptr += sizeof(bool);

        usize size = *(usize *)ptr;
        ptr += sizeof(usize);

        f32 height = *(f32 *)ptr;
        ptr += sizeof(f32);

        char *text = *(char *)ptr;
        ptr += sizeof(char) * size;

        if (is_title)
        {
            --remaining_titles;
            draw_text(text, x, y, DEBUG_PANEL_TITLE_FONT_SIZE, panel.font_title);
            y += DEBUG_PANEL_TITLE_SPACING;
        }
        else
        {
            --remaining_entries;
            draw_text(text, x, y, DEBUG_PANEL_ENTRY_FONT_SIZE, panel.font_entry);
        }

        y += height;
    }
}

void DebugPanelClean(DebugPanel *panel)
{
    panel->titles = 0;
    panel->entries = 0;
    ArenaClear(&panel->arena);
}
