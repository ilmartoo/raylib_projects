#include "entities/collisions.h"
#include "utils/extra_math.h"
#include "debug/game_debug.h"
#include "lifecycles/game_lifecycle.h"
#include "lifecycles/game_state.h"
#include "types/object_pool.h"
#include "raylib/raymath.h"

void GameUpdatePlayers(void);
void GameUpdateProyectiles(void);
void GameCheckCollisions(void);

void TestingInput(void);

// All the calculations that happen at every frame
void GameFrame(void) {
#ifdef DEBUG
    GameDebugInput();
#endif  // DEBUG
#ifdef TESTING
    TestingInput();
#endif  // TESTING

    if (state->time_running) {
        // State
        GameStateUpdate();

        // Entities
        GameUpdatePlayers();
        GameUpdateProyectiles();

        // Collisions
        GameCheckCollisions();
    }

#ifdef DEBUG
    GameDebugUpdate();
#endif
}

// Update players
void GameUpdatePlayers(void) {
    ForEachPlayerRef(iter) {
        Player* player = iter.player;
        if (!GameStateIsPlayerGameOver(*player)) {
            PlayerMove(player);
            PlayerAim(player);
            PlayerShootBasic(player);
            PlayerShootMissile(player);
        }
    }
}

// Update projectiles
void GameUpdateProyectiles(void) {
    ObjectPool* pool;
    ForEachObjectPoolObject(pool = &state->projectiles_players, Projectile, iter) {
        if (!ProjectileMove(iter.object)) { ObjectPoolObjectRemove(pool, iter.index); }
    }

    ForEachObjectPoolObject(pool = &state->projectiles_enemies, Projectile, iter) {
        if (!ProjectileMove(iter.object)) { ObjectPoolObjectRemove(pool, iter.index); }
    }
}

// Game collision checking
void GameCheckCollisions(void) {
    ObjectPool *enemies = &state->enemies, *projectiles_enemies = &state->projectiles_enemies, *projectiles_players = &state->projectiles_players;

    ForEachObjectPoolObject(projectiles_enemies, Projectile, iter_projectile) {
        bool collided = false;
        Projectile projectile = *iter_projectile.object;
        ForEachPlayerRef(iter_player) {
            Player* player = iter_player.player;
            if (CheckEntityCollision(player->entity, projectile.entity)) {
                PlayerDamage(player, projectile.damage);
                collided = true;
            }
        }
        if (collided) { ObjectPoolObjectRemove(projectiles_enemies, iter_projectile.index); }
    }

    ForEachObjectPoolObject(projectiles_players, Projectile, iter_projectile) {
        bool collided = false;
        Projectile projectile = *iter_projectile.object;
        ForEachObjectPoolObject(enemies, Enemy, iter_enemy) {
            Enemy* enemy = iter_enemy.object;
            if (CheckEntityCollision(enemy->entity, projectile.entity)) {
                if (EnemyDamage(enemy, projectile.damage)) { ObjectPoolObjectRemove(enemies, iter_enemy.index); }
                collided = true;
            }
        }
        if (collided) { ObjectPoolObjectRemove(projectiles_players, iter_projectile.index); }
    }
}

#ifdef TESTING

// Generate enemies around the player
void GameDebugGenerateEnemiesAroundPlayer(void) {
    ForEachObjectPoolObject(&state->enemies, Enemy, iter) { ObjectPoolObjectRemove(&state->enemies, iter.index); }  // Might remove later on

#define ENEMIES_CREATED_AT_START         4
#define ENEMIES_CREATED_AT_START_SPACING 400

    Vector2 player_center = EntityCenter(state->players[0].entity);

    for (u32 i = 0; i < ENEMIES_CREATED_AT_START; ++i) {
        f32 rotation = Deg2Rad(GetRandomValue(0, MAX_DEGS));
        Vector2 pos = Vector2Add(player_center, Vector2Scale(Vector2UnitCirclePoint(rotation), ENEMIES_CREATED_AT_START_SPACING));

        EnemyCreate(
            i & 1 ? SPACESHIP_ENEMY_UPGRADED : SPACESHIP_ENEMY_BASE, pos, ENEMY_MOVEMENT_SPEED, Vector2AngleFromXAxis(Vector2Subtract(player_center, pos)));
    }
}

// Inputs available for testing
void TestingInput(void) {
    // Pause time (PAUSE ?)
    if (IsKeyPressed(KEY_ZERO)) { state->time_running = !state->time_running; }

    // Generate random enemies around player 0
    if (IsKeyPressed(KEY_ONE)) { GameDebugGenerateEnemiesAroundPlayer(); }

    // Toggle bounding circles
    if (IsKeyPressed(KEY_TWO)) { state->testing_draw_bounding_circles = !state->testing_draw_bounding_circles; }

    // Toggle player rotation
    if (IsKeyPressed(KEY_THREE)) { state->testing_draw_player_rotation = !state->testing_draw_player_rotation; }

    // Toggle maximized window
    if (IsKeyPressed(KEY_SEVEN)) { MaximizeWindow(); }

    // Toggle borderless window
    if (IsKeyPressed(KEY_EIGHT)) { ToggleBorderlessWindowed(); }

    // Toggle fullscreen
    if (IsKeyPressed(KEY_NINE)) { ToggleFullscreen(); }

    // Game speed
    bool increase_speed = IsKeyPressed(KEY_PAGE_UP);
    bool decrease_speed = IsKeyPressed(KEY_PAGE_DOWN);
    if (increase_speed != decrease_speed) {
        if (increase_speed && state->time_speed_magnitude < GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX) { ++state->time_speed_magnitude; }
        if (decrease_speed && state->time_speed_magnitude > -GAME_STATE_TIME_SPEED_MAGNITUDE_ABSOLUTE_MAX) { --state->time_speed_magnitude; }
    }
}

#endif  // TESTING
