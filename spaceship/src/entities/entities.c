#include <math.h>

#include "abilities.h"
#include "debug.h"
#include "entities.h"
#include "rayheader.h"
#include "state.h"

#ifdef DEBUG
#define entity_bounding_box_draw(center, radius, color, fill)                                                                              \
    do {                                                                                                                                   \
        if (fill) { DrawCircleV(center, radius, color); }                                                                                  \
        else { DrawCircleLinesV(center, radius, color); }                                                                                  \
    } while (0)
#else
#define entity_bounding_box_draw(center, radius, color, fill)
#endif

Vector2 entity_center(Entity entity) { return Vector2Add(entity.position, Vector2Scale(entity.size, 0.5)); }

void entity_draw(Entity entity, Rectangle texture_location)
{
    Vector2 size_center = Vector2Scale(entity.size, 0.5);

    DrawTexturePro(state.spritesheet, texture_location,
                   (Rectangle){size_center.x + entity.position.x, size_center.y + entity.position.y, entity.size.x, entity.size.y},
                   size_center, Rad2Deg(entity.rotation + entity.draw_rotation), WHITE);
}

Projectile *projectile_create(ProjectileType type, Entity entity, u32 damage, u32 range)
{
    Projectile projectile = {
        .type = type,
        .entity = entity,
        .damage = damage,
        .range = range,
        .origin = entity.position,
    };

    return (Projectile *)data_object_pool_add((type == PROYECTILE_PLAYER ? &state.projectiles_players : &state.projectiles_enemies),
                                              &projectile);
}

bool projectile_move(Projectile *projectile)
{
    f32 distance = Vector2Distance(projectile->entity.position, projectile->origin);
    u32 range = projectile->range;
    if (distance < range && -distance < range)
    {
        projectile->entity.velocity = adjust_vector2_to_time_delta(projectile->entity.movement_speed);
        projectile->entity.position = Vector2Add(projectile->entity.position, projectile->entity.velocity);
        return true;
    }
    return false;
}

void proyectile_draw(Projectile projectile)
{
    entity_draw(projectile.entity, locate_texture_projectile(projectile.type));

    Vector2 center = entity_center(projectile.entity);
    Vector2 half_size = Vector2Scale(projectile.entity.size, 0.5);
    entity_bounding_box_draw(center, half_size.y, Fade(ORANGE, 0.5), false);
    entity_bounding_box_draw(
        Vector2Add(center, Vector2Scale(Vector2UnitCirclePoint(projectile.entity.rotation), half_size.y - half_size.x)), half_size.x, RED,
        true);
}

Player *player_create(SpaceshipType type, Vector2 position)
{
    Player player = {.entity =
                         {
                             .position = position,
                             .size = PLAYER_SIZE,
                             .movement_speed = PLAYER_MOVEMENT_SPEED,
                             .velocity = {0},
                             .rotation = 0,
                             .draw_rotation = PLAYER_DRAW_ROTATION,
                         },
                     .type = type,
                     .aiming_at = {0},
                     .alternate_shooting = true,
                     .using_gamepad = true,
                     .controls =
                         {
                             {
                                 .device = INPUT_DEVICE_KEYBOARD_AND_MOUSE,
                                 .actions = DEFAULT_MOUSE_AND_KEYBOARD_INPUTS,
                             },
                             {
                                 .device = 0,
                                 .actions = DEFAULT_GAMEPAD_INPUTS,
                             },
                         },
                     .shoot_basic = ABILITY_SHOOT,
                     .shoot_missile = ABILITY_SHOOT_MISSILE};

    return (Player *)data_object_pool_add(&state.players, &player);
}

f32 player_input_keyboard_and_mouse(Player player, Input input)
{
    switch (input.method)
    {
    case INPUT_METHOD_MOUSE_BUTTON: return mouse_button_down(input.value);
    case INPUT_METHOD_KEYBOARD_KEY: return keyboard_key_down(input.value);
    case INPUT_METHOD_POINTER_POSITION:
        return pointer_position_offset(input.value, entity_center(player.entity), PLAYER_AIMING_SIGHT_MAX_DISTANCE);
    default: return 0;
    }
}

f32 player_input_gamepad(InputDevice gamepad_id, Input input)
{
    switch (input.method)
    {
    case INPUT_METHOD_GAMEPAD_BUTTON: return gamepad_button_down(gamepad_id, input.value);
    case INPUT_METHOD_GAMEPAD_TRIGGER: return gamepad_trigger_pressure(gamepad_id, input.value);
    case INPUT_METHOD_GAMEPAD_JOYSTICK: return gamepad_joystick_offset(gamepad_id, input.value);
    default: return 0;
    }
}

f32 player_input(Player player, PlayerAction action)
{
    PlayerControls controls = player.controls[player.using_gamepad];
    Input action_input = controls.actions[action];

    switch (controls.device)
    {
    case INPUT_DEVICE_NO_GAMEPAD: return 0;

    case INPUT_DEVICE_KEYBOARD_AND_MOUSE: return player_input_keyboard_and_mouse(player, action_input);

    default: return player_input_gamepad(controls.device, action_input);
    }
}

void player_move(Player *player)
{
    f32 move_x = player_input(*player, ACTION_MOVE_RIGHT) - player_input(*player, ACTION_MOVE_LEFT);
    f32 move_y = player_input(*player, ACTION_MOVE_DOWN) - player_input(*player, ACTION_MOVE_UP);

    player->entity.velocity = Vector2Zero();

    if (move_x > PLAYER_THRESHOLD_MOVEMENT || move_x < -PLAYER_THRESHOLD_MOVEMENT)
    {
        player->entity.position.x += player->entity.velocity.x = adjust_to_time_delta(move_x * player->entity.movement_speed.x);
    }
    if (move_y > PLAYER_THRESHOLD_MOVEMENT || move_y < -PLAYER_THRESHOLD_MOVEMENT)
    {
        player->entity.position.y += player->entity.velocity.y = adjust_to_time_delta(move_y * player->entity.movement_speed.y);
    }
}

void player_aim(Player *player)
{
    Vector2 aim_delta = {player_input(*player, ACTION_AIM_RIGHT) - player_input(*player, ACTION_AIM_LEFT),
                         player_input(*player, ACTION_AIM_DOWN) - player_input(*player, ACTION_AIM_UP)};

    f32 distance = Vector2Length(aim_delta);
    f32 threshold = PLAYER_AIMING_SIGHT_MIN_DISTANCE / (f32)PLAYER_AIMING_SIGHT_MAX_DISTANCE;

    if (distance < threshold)
    {
        Vector2 aim_position = Vector2Scale(Vector2UnitCirclePoint(player->entity.rotation), PLAYER_AIMING_SIGHT_MIN_DISTANCE);

        player->aiming_at = Vector2Add(aim_position, entity_center(player->entity));
    }
    else
    {
        f32 angle = Vector2Angle((Vector2){1, 0}, aim_delta); // Angle with Y-AXIS
        if (angle < 0) { angle += TAU; }

        Vector2 aim_position = distance > 1 ? Vector2Scale(Vector2UnitCirclePoint(angle), PLAYER_AIMING_SIGHT_MAX_DISTANCE)
                                            : Vector2Scale(aim_delta, PLAYER_AIMING_SIGHT_MAX_DISTANCE);
        aim_position = Vector2Add(aim_position, entity_center(player->entity));

        player->aiming_at = aim_position;
        player->entity.rotation = angle;
    }
}

void player_shoot(Player *player, AbilityProjectile *ability, PlayerAction action, Vector2 projectile_size, f32 projectile_speed)
{
    if (is_cooldown_ready(ability->cooldown))
    {
        bool is_shooting = CeilNormalizedValue(player_input(*player, action));

        if (is_shooting)
        {
            f32 rotation = player->entity.rotation;

            Vector2 rotation_vector = Vector2UnitCirclePoint(rotation + PI_HALF); // To offset the shoot horizontally
            Vector2 offset = Vector2Scale(rotation_vector, player->alternate_shooting ? -PLAYER_CANNONS_OFFSET : PLAYER_CANNONS_OFFSET);
            offset = Vector2Subtract(offset, Vector2Scale(projectile_size, 0.5));

            Vector2 position = Vector2Add(entity_center(player->entity), offset);

            projectile_create(PROYECTILE_PLAYER,
                              (Entity){
                                  .position = position,
                                  .size = projectile_size,
                                  .movement_speed = Vector2Scale(Vector2UnitCirclePoint(rotation), projectile_speed),
                                  .velocity = {0},
                                  .rotation = rotation,
                                  .draw_rotation = PROJECTILE_DRAW_ROTATION,
                              },
                              ability->damage, ability->range);

            cooldown_start(&ability->cooldown);
            player->alternate_shooting = !player->alternate_shooting;
        }
    }
}

void player_shoot_basic(Player *player)
{
    player_shoot(player, &player->shoot_basic, ACTION_ABILITY_SHOOT, PROJECTILE_BASIC_SIZE, PROJECTILE_BASIC_SPEED);
}

void player_shoot_missile(Player *player)
{
    player_shoot(player, &player->shoot_missile, ACTION_ABILITY_SHOOT_MISSILE, PROJECTILE_MISSILE_SIZE, PROJECTILE_MISSILE_SPEED);
}

void player_aiming_sight_draw(Player player)
{
    Vector2 half_size = Vector2Scale(PLAYER_AIMING_SIGHT_SIZE, 0.5);

    Vector2 points[3] = {{-half_size.x, half_size.y}, {0, -half_size.y}, {half_size.x, half_size.y}};
    Vector2 rotation_vector = Vector2UnitCirclePoint(player.entity.rotation);

    for (u32 i = 0; i < 3; ++i) { points[i] = Vector2Subtract(player.aiming_at, Vector2Multiply(rotation_vector, points[i])); }
    // for (u32 i = 0; i < 3; ++i) { points[i] = Vector2Add(points[i], player.aiming_at); }

    DrawLineEx(points[0], points[1], PLAYER_AIMING_SIGHT_THICKNESS, WHITE);
    DrawLineEx(points[2], points[1], PLAYER_AIMING_SIGHT_THICKNESS, WHITE);
}

#ifdef DEBUG
#define player_rotation_draw(player)                                                                                                       \
    do {                                                                                                                                   \
        Font __font = state.fonts[0];                                                                                                      \
        i32 __font_size = 16;                                                                                                              \
                                                                                                                                           \
        const char *__texts[8] = {"0 PI\n0 deg",   "1/4 PI\n45 deg",  "2/4 PI\n90 deg",  "3/4 PI\n135 deg",                                \
                                  "1 PI\n180 deg", "5/4 PI\n225 deg", "6/4 PI\n270 deg", "7/4 PI\n315 deg"};                               \
        f32 __rads = 0;                                                                                                                    \
                                                                                                                                           \
        Vector2 __entity_center = entity_center(player.entity);                                                                            \
        f32 __radius = PLAYER_AIMING_SIGHT_MIN_DISTANCE * 2;                                                                               \
                                                                                                                                           \
        for (u32 __i = 0; __i < 8; ++__i, __rads += PI_QUARTER)                                                                            \
        {                                                                                                                                  \
            const char *__text = __texts[__i];                                                                                             \
            Vector2 __pos = {cosf(__rads), sinf(__rads)};                                                                                  \
            __pos = Vector2Scale(__pos, __radius);                                                                                         \
            __pos = Vector2Add(__pos, __entity_center);                                                                                    \
                                                                                                                                           \
            Vector2 __text_measure = MeasureTextEx(__font, __text, __font_size, -1);                                                       \
            Vector2 __text_center = Vector2Scale(__text_measure, 0.5);                                                                     \
                                                                                                                                           \
            __pos.y -= __text_measure.y * 0.5;                                                                                             \
            switch (__i)                                                                                                                   \
            {                                                                                                                              \
            case 2:                                                                                                                        \
            case 6: __pos.x -= __text_center.x; break;                                                                                     \
            case 3:                                                                                                                        \
            case 4:                                                                                                                        \
            case 5: __pos.x -= __text_measure.x; break;                                                                                    \
            }                                                                                                                              \
                                                                                                                                           \
            DrawLineV(__entity_center, Vector2Add(__text_center, __pos), Fade(SKYBLUE, 0.25));                                             \
            DrawTextEx(__font, __text, __pos, __font_size, -1, BLUE);                                                                      \
        }                                                                                                                                  \
    } while (0)
#else
#define player_rotation_draw(player)
#endif

void player_draw(Player player)
{
    entity_draw(player.entity, locate_texture_spaceship(player.type));
    player_aiming_sight_draw(player);
    player_rotation_draw(player);
}

Enemy *enemy_create(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation)
{
    Enemy enemy = {.entity = {.position = position, .size = ENEMY_SIZE, .movement_speed = movement_speed, .rotation = rotation},
                   .type = type,
                   .shoot_basic = ABILITY_SHOOT};

    return (Enemy *)data_object_pool_add(&state.enemies, &enemy);
}

void enemy_draw(Enemy enemy)
{
    DrawTexturePro(state.spritesheet, locate_texture_spaceship(enemy.type),
                   (Rectangle){enemy.entity.position.x, enemy.entity.position.y, enemy.entity.size.x, enemy.entity.size.y}, (Vector2){0},
                   enemy.entity.rotation, WHITE);
}
