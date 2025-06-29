#include <math.h>

#include "abilities.h"
#include "actions.h"
#include "debug.h"
#include "entities.h"
#include "extra_math.h"
#include "game_state.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Entity ----------------------------------------------------------------
// ----------------------------------------------------------------------------

void _EntityBoundingCircleDraw(Entity entity, Color color, bool fill)
{
    if (state->testing_draw_bounding_circles)
    {
        if (fill) { DrawCircleV(EntityBoundingCircleCenter(entity), entity.bounding_circle.radius, color); }
        else { DrawCircleLinesV(EntityBoundingCircleCenter(entity), entity.bounding_circle.radius, color); }
    }
}

void EntitySetRotation(Entity *entity, f32 rotation)
{
    entity->rotation = rotation;
    entity->bounding_circle._center_rotated = Vector2Rotate(entity->bounding_circle.center, rotation + entity->_draw_rotation);
}

Vector2 EntityCenter(Entity entity) { return Vector2Add(entity.position, Vector2Scale(entity.size, 0.5)); }

Vector2 EntityBoundingCircleCenter(Entity entity) { return Vector2Add(EntityCenter(entity), entity.bounding_circle._center_rotated); }

void EntityDraw(Entity entity, Rectangle texture_location)
{
    Vector2 size_center = Vector2Scale(entity.size, 0.5);

    DrawTexturePro(state->spritesheet,
                   texture_location,
                   (Rectangle){size_center.x + entity.position.x, size_center.y + entity.position.y, entity.size.x, entity.size.y},
                   size_center,
                   Rad2Deg(entity.rotation + entity._draw_rotation),
                   WHITE);

    _EntityBoundingCircleDraw(entity, Fade(LIME, 0.5), false);
}

// ----------------------------------------------------------------------------
// ---- Health ----------------------------------------------------------------
// ----------------------------------------------------------------------------

Health HealthCreate(u32 max) { return (Health){.current = max, .max = max}; }

void HealthBarDraw(Health health, Entity entity, Color border_color, Color health_color)
{
    Vector2 health_bar_size = Vector2From(entity.size.x, HEALTH_BAR_HEIGTH);

    Vector2 border_draw_position = Vector2Subtract(EntityCenter(entity), Vector2Scale(health_bar_size, 0.5f));
    border_draw_position.y -= (entity.size.y * 0.5f) + HEALTH_BAR_HEIGTH + HEALTH_BAR_SEPARATION;

    Vector2 health_draw_position = Vector2AddValue(border_draw_position, HEALTH_BAR_BORDER_WIDTH);

    Vector2 health_size = Vector2AddValue(health_bar_size, -HEALTH_BAR_BORDER_WIDTH * 2);
    health_size.x *= (health.current / (f32)health.max);

    DrawRectangleV(border_draw_position, health_bar_size, Fade(border_color, HEALTH_BAR_OPACITY));
    DrawRectangleV(health_draw_position, health_size, health_color);
}

// ----------------------------------------------------------------------------
// ---- Proyectile ------------------------------------------------------------
// ----------------------------------------------------------------------------

Projectile *ProjectileCreate(ProjectileType type,
                             Vector2 position,
                             Vector2 size,
                             f32 projectile_speed,
                             f32 rotation,
                             BoundingCircle bounding_circle,
                             u32 damage,
                             u32 range)
{
    Projectile projectile = {
        .type = type,
        .entity =
            {
                .position = position,
                .size = size,
                .movement_speed = Vector2Scale(Vector2UnitCirclePoint(rotation), projectile_speed),
                .velocity = {0},
                .rotation = rotation,
                ._draw_rotation = PROJECTILE_DRAW_ROTATION,
                .bounding_circle = bounding_circle,
            },
        .damage = damage,
        .range = range,

        ._origin = position,
    };

    return (Projectile *)ObjectPoolObjectAdd((type == PROYECTILE_PLAYER ? &state->projectiles_players : &state->projectiles_enemies),
                                       &projectile);
}

bool ProjectileMove(Projectile *projectile)
{
    f32 distance = Vector2Distance(projectile->entity.position, projectile->_origin);
    u32 range = projectile->range;
    if (distance < range)
    {
        projectile->entity.velocity = Vector2ScaleToDelta(projectile->entity.movement_speed);
        projectile->entity.position = Vector2Add(projectile->entity.position, projectile->entity.velocity);
        return true;
    }
    return false;
}

void ProjectileDraw(Projectile projectile) { EntityDraw(projectile.entity, ProjectileTextureLocation(projectile.type)); }

// ----------------------------------------------------------------------------
// ---- Player ----------------------------------------------------------------
// ----------------------------------------------------------------------------

Player PlayerCreate(InputDevice device, u8 player_index, SpaceshipType type, Vector2 position)
{
    f32 rotation = 0;
    return (Player){
        .entity = {.position = position,
                   .size = PLAYER_SIZE,
                   .movement_speed = PLAYER_MOVEMENT_SPEED,
                   .velocity = {0},
                   .rotation = rotation,
                   ._draw_rotation = PLAYER_DRAW_ROTATION,
                   .bounding_circle = PLAYER_BOUNDING_CIRCLE(rotation)},
        .type = type,
        .health = PLAYER_HEALTH,

        .camera = PLAYER_CAMERA(position),

        .ability_regeneration = PLAYER_ABILITY_REGENERATION,
        .ability_shooting = PLAYER_ABILITY_SHOOT,
        .ability_missile = PLAYER_ABILITY_MISSILE,

        ._player_index = player_index,
        ._alternate_shooting = true,
        ._device = device,
    };
}

f32 PlayerGetAction(Player player, Action action)
{
    InputDevice device = player._device;
    Mapping mapping = state->mappings[device][action];

    if (device == INPUT_DEVICE_MOUSE_AND_KEYBOARD)
    {
        switch (mapping.method)
        {
        case INPUT_METHOD_MOUSE_BUTTON: return IsInputMouseButtonDown(mapping.value);
        case INPUT_METHOD_KEYBOARD_KEY: return IsInputKeyboardKeyDown(mapping.value);
        case INPUT_METHOD_CURSOR_POSITION: return InputMouseCursorPosition(mapping.value, EntityCenter(player.entity));
        default: break;
        }
    }
    else if (device >= 0)
    {
        switch (mapping.method)
        {
        case INPUT_METHOD_GAMEPAD_BUTTON: return IsInputGamepadButtonDown(device, mapping.value);
        case INPUT_METHOD_GAMEPAD_TRIGGER: return InputGamepadTriggerPressure(device, mapping.value);
        case INPUT_METHOD_GAMEPAD_JOYSTICK: return InputGamepadJoystickOffset(device, mapping.value);
        default: break;
        }
    }
    return 0;
}

void PlayerMove(Player *player)
{
    f32 move_x = PlayerGetAction(*player, ACTION_MOVE_RIGHT) - PlayerGetAction(*player, ACTION_MOVE_LEFT);
    f32 move_y = PlayerGetAction(*player, ACTION_MOVE_DOWN) - PlayerGetAction(*player, ACTION_MOVE_UP);

    player->entity.velocity = Vector2Zero();

    if (move_x > PLAYER_ACTION_MOVE_THRESHOLD || move_x < -PLAYER_ACTION_MOVE_THRESHOLD)
    {
        player->entity.position.x += player->entity.velocity.x = ScaleToDelta(move_x * player->entity.movement_speed.x);
    }
    if (move_y > PLAYER_ACTION_MOVE_THRESHOLD || move_y < -PLAYER_ACTION_MOVE_THRESHOLD)
    {
        player->entity.position.y += player->entity.velocity.y = ScaleToDelta(move_y * player->entity.movement_speed.y);
    }
}

void PlayerAim(Player *player)
{
    Vector2 aim_delta = {PlayerGetAction(*player, ACTION_AIM_RIGHT) - PlayerGetAction(*player, ACTION_AIM_LEFT),
                         PlayerGetAction(*player, ACTION_AIM_DOWN) - PlayerGetAction(*player, ACTION_AIM_UP)};

    if (aim_delta.x == 0 && aim_delta.y == 0)
    {
        aim_delta = player->entity.velocity;

        if (aim_delta.x == 0 && aim_delta.y == 0) { return; } // If the player is static the rotation should not change
    }

    f32 angle = Vector2AngleFromXAxis(aim_delta); // Angle from X-AXIS
    if (angle < 0) { angle += TAU; }

    EntitySetRotation(&player->entity, angle);
}

void PlayerShoot(Player *player,
                 AbilityProjectile *ability,
                 Action action,
                 BoundingCircle bounding_circle,
                 Vector2 projectile_size,
                 f32 projectile_speed)
{
    if (IsCooldownReady(ability->cooldown))
    {
        bool is_shooting = CeilNormalizedValue(PlayerGetAction(*player, action));

        if (is_shooting)
        {
            f32 rotation = player->entity.rotation;

            Vector2 rotation_vector = Vector2UnitCirclePoint(rotation + PI_HALF); // To offset the shoot horizontally
            Vector2 offset = Vector2Scale(rotation_vector, player->_alternate_shooting ? -PLAYER_CANNONS_OFFSET : PLAYER_CANNONS_OFFSET);
            offset = Vector2Subtract(offset, Vector2Scale(projectile_size, 0.5));

            Vector2 position = Vector2Add(EntityCenter(player->entity), offset);

            ProjectileCreate(
                PROYECTILE_PLAYER, position, projectile_size, projectile_speed, rotation, bounding_circle, ability->damage, ability->range);

            CooldownStart(&ability->cooldown);
            player->_alternate_shooting = !player->_alternate_shooting;
        }
    }
}

void PlayerShootBasic(Player *player)
{
    PlayerShoot(player,
                &player->ability_shooting,
                ACTION_ABILITY_SHOOT,
                PROJECTILE_BASIC_BOUNDING_CIRCLE(player->entity.rotation),
                PROJECTILE_BASIC_SIZE,
                PROJECTILE_BASIC_SPEED);
}

void PlayerShootMissile(Player *player)
{
    PlayerShoot(player,
                &player->ability_missile,
                ACTION_ABILITY_MISSILE,
                PROJECTILE_MISSILE_BOUNDING_CIRCLE(player->entity.rotation),
                PROJECTILE_MISSILE_SIZE,
                PROJECTILE_MISSILE_SPEED);
}

void PlayerDamage(Player *player, u32 damage)
{
    if (player->health.current <= damage)
    {
        player->health.current = 0;
        GameStateSetPlayerGameOver(player);
    }
    else { player->health.current -= damage; }
}

void _PlayerRotationDraw(Player player)
{
    if (state->testing_draw_player_rotation)
    {
        Font font = state->font;
        i32 font_size = 16;

        const char *texts[8] = {"0 PI\n0 deg",
                                "1/4 PI\n45 deg",
                                "2/4 PI\n90 deg",
                                "3/4 PI\n135 deg",
                                "1 PI\n180 deg",
                                "5/4 PI\n225 deg",
                                "6/4 PI\n270 deg",
                                "7/4 PI\n315 deg"};
        f32 rads = 0;

        Vector2 entity_center = EntityCenter(player.entity);
        f32 radius = TESTING_PLAYER_ROTATION_DIAGRAM_DISTANCE;

        for (u32 i = 0; i < 8; ++i, rads += PI_QUARTER)
        {
            const char *text = texts[i];
            Vector2 pos = {cosf(rads), sinf(rads)};
            pos = Vector2Scale(pos, radius);
            pos = Vector2Add(pos, entity_center);

            Vector2 text_measure = MeasureTextEx(font, text, font_size, -1);
            Vector2 text_center = Vector2Scale(text_measure, 0.5);

            pos.y -= text_measure.y * 0.5;
            switch (i)
            {
            case 2:
            case 6: pos.x -= text_center.x; break;
            case 3:
            case 4:
            case 5: pos.x -= text_measure.x; break;
            }

            DrawLineV(entity_center, Vector2Add(text_center, pos), Fade(SKYBLUE, 0.25));
            DrawTextEx(font, text, pos, font_size, -1, BLUE);
        }
    }
}

void PlayerDraw(Player player)
{
    EntityDraw(player.entity, SpaceshipTextureLocation(player.type));
    _PlayerRotationDraw(player);
}

// ----------------------------------------------------------------------------
// ---- Enemy -----------------------------------------------------------------
// ----------------------------------------------------------------------------

Enemy *EnemyCreate(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation)
{
    Enemy enemy = {.entity = {.position = position,
                              .size = ENEMY_SIZE,
                              .movement_speed = movement_speed,
                              .rotation = rotation,
                              ._draw_rotation = ENEMY_DRAW_ROTATION,
                              .bounding_circle = ENEMY_BOUNDING_CIRCLE(rotation)},
                   .type = type,
                   .health = ENEMY_HEALTH,

                   .ability_shooting = ENEMY_ABILITY_SHOOT};

    return (Enemy *)ObjectPoolObjectAdd(&state->enemies, &enemy);
}

bool EnemyDamage(Enemy *enemy, u32 damage)
{
    if (enemy->health.current <= damage)
    {
        enemy->health.current = 0;
        return true;
    }
    else
    {
        enemy->health.current -= damage;
        return false;
    }
}

void EnemyDraw(Enemy enemy)
{
    EntityDraw(enemy.entity, SpaceshipTextureLocation(enemy.type));
    HealthBarDraw(enemy.health, enemy.entity, MAROON, RED);
}
