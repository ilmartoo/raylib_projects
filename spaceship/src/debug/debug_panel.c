#ifdef DEBUG

#include <stdlib.h>

#include "arena.h"
#include "debug_panel.h"
#include "memory_utils.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"

DebugPanel *DebugPanelCreate(Color background_color, Font font)
{
    DebugPanel *panel = reserve(DebugPanel);
    *panel = (DebugPanel){
        .arena = ArenaCreate(), .background_color = background_color, .font = font, .titles = 0, .entries = 0, .content_size = {0}};
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

    *ArenaPushType(&panel->arena, bool) = true;            // Is a title
    *ArenaPushTypeZero(&panel->arena, usize) = title_size; // Text size
    f32 *height = ArenaPushTypeZero(&panel->arena, f32);
    char *title_mem = ArenaPushArrayZero(&panel->arena, char, title_size);

    if (title != NULL)
    {
        memory_copy(title_mem, title, title_size - 1);
        Vector2 title_measures = MeasureTextEx(panel->font, title, DEBUG_PANEL_TITLE_FONT_SIZE, DEBUG_PANEL_FONT_SPACING);
        panel->content_size.x = max(panel->content_size.x, title_measures.x);
        panel->content_size.y += *height = title_measures.y;
    }
}

void DebugPanelAddEntry(DebugPanel *panel, const char *text)
{
    ++panel->entries;

    usize text_size = TextLength(text) + 1;

    *ArenaPushType(&panel->arena, bool) = false;          // Is an entry
    *ArenaPushTypeZero(&panel->arena, usize) = text_size; // Text size
    f32 *height = ArenaPushTypeZero(&panel->arena, f32);
    char *text_mem = ArenaPushArrayZero(&panel->arena, char, text_size);

    if (text != NULL)
    {
        memory_copy(text_mem, text, text_size - 1);
        Vector2 text_measures = MeasureTextEx(panel->font, text, DEBUG_PANEL_ENTRY_FONT_SIZE, DEBUG_PANEL_FONT_SPACING);
        panel->content_size.x = max(panel->content_size.x, text_measures.x);
        panel->content_size.y += *height = text_measures.y;
    }
}

void _DebugPanelTextDraw(const char *text, f32 x, f32 y, float font_size, Font font)
{
    DrawTextEx(font, text, (Vector2){x, y}, font_size, 0, WHITE);
}

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
    uptr ptr = (uptr)panel.arena.memory;

    while (remaining_titles > 0 || remaining_entries > 0)
    {
        bool is_title = *(bool *)ptr;

        ptr += sizeof(bool);
        usize size = *(usize *)ptr;
        ptr += sizeof(usize);

        f32 height = *(f32 *)ptr;
        ptr += sizeof(f32);

        char *text = (char *)ptr;
        ptr += sizeof(char) * size;

        f32 font_size;
        if (is_title)
        {
            font_size = DEBUG_PANEL_TITLE_FONT_SIZE;
            y += DEBUG_PANEL_TITLE_SPACING;
            --remaining_titles;
        }
        else
        {
            font_size = DEBUG_PANEL_ENTRY_FONT_SIZE;
            --remaining_entries;
        }
        _DebugPanelTextDraw(text, x, y, font_size, panel.font);

        y += height;
    }
}

void DebugPanelClean(DebugPanel *panel)
{
    panel->titles = 0;
    panel->entries = 0;
    panel->content_size = Vector2Zero();
    ArenaClear(&panel->arena);
}

#endif // DEBUG
