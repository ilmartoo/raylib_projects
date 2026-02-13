#include "entities/entities.h"
#include "debug/game_debug.h"
#include "lifecycles/game_lifecycle.h"
#include "lifecycles/game_state.h"
#include "types/object_pool.h"

// Game draw
void GameDraw(void) {
    /* Start */ BeginDrawing();

    ClearBackground(BLANK);

    // Players
    ForEachPlayerVal(iter) { PlayerDraw(iter.player); }

    // Enemies
    ForEachObjectPoolObject(&state->enemies, Enemy, iter) { EnemyDraw(*iter.object); }

    // Proyectiles
    ForEachObjectPoolObject(&state->projectiles_players, Projectile, iter) { ProjectileDraw(*iter.object); }
    ForEachObjectPoolObject(&state->projectiles_enemies, Projectile, iter) { ProjectileDraw(*iter.object); }

#ifdef DEBUG
    GameDebugDraw();
#else
    DrawFPS(10, 10);
#endif

    /* End */ EndDrawing();
}
