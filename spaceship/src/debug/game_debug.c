#ifdef DEBUG

#include "game_debug.h"
#include "debug_panel.h"
#include "game_state.h"

DebugPanel *timings_panel;
DebugPanel *entities_panel;
DebugPanel *inputs_panel;

// Debug panels initialization
void GameDebugInitialize(void)
{
    timings_panel = DebugPanelCreate(DARKGREEN, state->font);
    entities_panel = DebugPanelCreate(GREEN, state->font);
    inputs_panel = DebugPanelCreate(ORANGE, state->font);
}

// Debug input check
void GameDebugInput(void) { /* Empty for now */ }

// Debug update
void GameDebugUpdate(void)
{
    DebugPanelClean(timings_panel);
    DebugPanelClean(entities_panel);
    DebugPanelClean(inputs_panel);

    DebugPanelAddTitle(timings_panel, "TIMINGS");
    DebugPanelAddEntry(timings_panel, TextFormat("%d fps", GetFPS()));
    DebugPanelAddEntry(timings_panel, TextFormat("%d%% speed", 100 + 20 * state->time_speed_magnitude));
    DebugPanelAddEntry(timings_panel, TextFormat("Game %s", state->time_running ? "running" : "paused"));

    ForEachPlayerVal(iter)
    {
        DebugPanelAddTitle(entities_panel, TextFormat("PLAYER %d", iter.index));
        DebugPanelAddEntry(entities_panel, TextFormat("Rotation: %.2f deg", Rad2Deg(iter.player.entity.rotation)));

        Vector2 velocity = iter.player.entity.velocity;
        DebugPanelAddEntry(entities_panel, TextFormat("Velocity: (%.2f, %.2f)", velocity.x, velocity.y));
    }

    DebugPanelAddTitle(entities_panel, "PLAYER PROYECTILES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state->projectiles_players)));
    DebugPanelAddEntry(entities_panel, TextFormat("Valid: %u", ObjectPoolObjectCount(state->projectiles_players)));

    DebugPanelAddTitle(entities_panel, "ENEMIES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state->enemies)));
    DebugPanelAddEntry(entities_panel, TextFormat("Valid: %u", ObjectPoolObjectCount(state->enemies)));

    DebugPanelAddTitle(entities_panel, "ENEMY PROYECTILES");
    DebugPanelAddEntry(entities_panel, TextFormat("Chunks: %u", ObjectPoolChunkCount(state->projectiles_enemies)));
    DebugPanelAddEntry(entities_panel, TextFormat("Valid: %u", ObjectPoolObjectCount(state->projectiles_enemies)));

    DebugPanelAddTitle(inputs_panel, "TESTING INPUTS");
    DebugPanelAddEntry(inputs_panel, "1 >> Generate enemies around player");
    DebugPanelAddEntry(inputs_panel, "2 >> Show/Hide bounding boxes");
    DebugPanelAddEntry(inputs_panel, "3 >> Show/Hide player rotation lines");
    DebugPanelAddEntry(inputs_panel, "7 >> Maximize window");
    DebugPanelAddEntry(inputs_panel, "8 >> Toggle borderless window");
    DebugPanelAddEntry(inputs_panel, "9 >> Toggle fullscreen window");
    DebugPanelAddEntry(inputs_panel, "0 >> Pause/Resume game");
    DebugPanelAddEntry(inputs_panel, "Page Up >> Increase game speed");
    DebugPanelAddEntry(inputs_panel, "Page Down >> Decrease game speed");
}

// Debug draw
void GameDebugDraw(void)
{
    DebugPanelDraw(*timings_panel, DEBUG_PANEL_TIMINGS_POSITION);
    DebugPanelDraw(*entities_panel, DEBUG_PANEL_ENTITIES_POSITION);
    DebugPanelDraw(*inputs_panel, DEBUG_PANEL_INPUTS_POSITION);
}

// Debug clear
void GameDebugClear(void)
{
    DebugPanelDelete(timings_panel);
    DebugPanelDelete(entities_panel);
    DebugPanelDelete(inputs_panel);
}

#endif // DEBUG
