#include <math.h>

#include "abilities.h"
#include "debug.h"
#include "entities.h"
#include "mathematics.h"
#include "raymath.h"
#include "state.h"

Vector2 entity_center(Entity entity) { return Vector2Add(entity.position, Vector2Scale(entity.size, 0.5)); }

#ifdef DEBUG
void draw_entity_bounding_box(Entity entity, Color color)
{
    DrawCircleLinesV(entity_center(entity), max(entity.size.x, entity.size.y) * 0.5, color);
}
#endif

void draw_entity(Entity entity, Rectangle texture_location)
{
    Vector2 size_center = Vector2Scale(entity.size, 0.5);

    DrawTexturePro(state.spritesheet, texture_location,
                   (Rectangle){size_center.x + entity.position.x, size_center.y + entity.position.y, entity.size.x, entity.size.y},
                   size_center, rad_to_deg(entity.rotation), WHITE);

#ifdef DEBUG
    draw_entity_bounding_box(entity, RED);
#endif
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
        projectile->entity.position = Vector2Subtract(projectile->entity.position, projectile->entity.velocity);
        return true;
    }
    return false;
}

void proyectile_draw(Projectile projectile) { draw_entity(projectile.entity, locate_texture_projectile(projectile.type)); }

Player *player_create(SpaceshipType type, Vector2 position)
{
    Player player = {.entity = {.position = position,
                                .size = PLAYER_SIZE,
                                .movement_speed = PLAYER_MOVEMENT_SPEED,
                                .velocity = {0},
                                .rotation = PLAYER_ROTATION},
                     .type = type,
                     .aiming_at = {0},
                     .shoot_from_left = true,
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
                     .shoot = ABILITY_SHOOT,
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
        Vector2 aim_position = Vector2Scale(vector2_unit_at_angle(player->entity.rotation), PLAYER_AIMING_SIGHT_MIN_DISTANCE);

        player->aiming_at = Vector2Add(aim_position, entity_center(player->entity));
    }
    else
    {
        f32 angle = Vector2Angle((Vector2){-1, 0}, aim_delta) + PLAYER_ROTATION; // Angle with Y-AXIS

        Vector2 aim_position = distance > 1 ? Vector2Scale(vector2_unit_at_angle(angle), PLAYER_AIMING_SIGHT_MAX_DISTANCE)
                                            : Vector2Scale(aim_delta, PLAYER_AIMING_SIGHT_MAX_DISTANCE);
        aim_position = Vector2Add(aim_position, entity_center(player->entity));

        player->aiming_at = aim_position;
        player->entity.rotation = angle;
    }
}

void player_shoot(Player *player)
{
    AbilityProjectile *shoot_ab = &player->shoot;
    if (is_cooldown_ready(shoot_ab->cooldown))
    {
        bool is_shooting = unit_ceil(player_input(*player, ACTION_ABILITY_SHOOT));

        if (is_shooting)
        {
            Vector2 position = entity_center(player->entity);
            f32 rotation = player->entity.rotation + PI;

            Vector2 unit_vector_movement_speed = vector2_unit_at_angle(rotation);
            Vector2 unit_vector_position = vector2_unit_at_angle(rotation + (PI * 0.5));

            Vector2 offset_position =
                Vector2Scale(unit_vector_position, player->shoot_from_left ? -PLAYER_CANNONS_OFFSET : PLAYER_CANNONS_OFFSET);
            position = Vector2Add(position, offset_position);

            projectile_create(PROYECTILE_PLAYER,
                              (Entity){
                                  .position = position,
                                  .size = PROJECTILE_STANDARD_SIZE,
                                  .movement_speed = Vector2Scale(unit_vector_movement_speed, PROJECTILE_STANDARD_SPEED),
                                  .velocity = {0},
                                  .rotation = rotation + PROJECTILE_ROTATION,
                              },
                              shoot_ab->damage, shoot_ab->range);

            cooldown_start(&shoot_ab->cooldown);
            player->shoot_from_left = !player->shoot_from_left;
        }
    }
}

void player_shoot_missile(Player *player)
{
    AbilityProjectile *missile_ab = &player->shoot_missile;
    if (is_cooldown_ready(missile_ab->cooldown))
    {
        bool is_shooting_missile = unit_ceil(player_input(*player, ACTION_ABILITY_SHOOT_MISSILE));

        if (is_shooting_missile)
        {
            Vector2 position = entity_center(player->entity);
            f32 rotation = player->entity.rotation + PI;

            Vector2 unit_vector_movement_speed = vector2_unit_at_angle(rotation);
            Vector2 unit_vector_position = vector2_unit_at_angle(rotation + (PI * 0.5));

            Vector2 offset_position =
                Vector2Scale(unit_vector_position, player->shoot_from_left ? -PLAYER_CANNONS_OFFSET : PLAYER_CANNONS_OFFSET);
            position = Vector2Add(position, offset_position);

            projectile_create(PROYECTILE_PLAYER,
                              (Entity){
                                  .position = position,
                                  .size = PROJECTILE_MISSILE_SIZE,
                                  .movement_speed = Vector2Add(player->entity.velocity,
                                                               Vector2Scale(unit_vector_movement_speed, PROJECTILE_MISSILE_SPEED)),
                                  .velocity = {0},
                                  .rotation = rotation + PROJECTILE_ROTATION,
                              },
                              missile_ab->damage, missile_ab->range);

            cooldown_start(&missile_ab->cooldown);
            player->shoot_from_left = !player->shoot_from_left;
        }
    }
}

void player_aiming_sight_draw(Player player)
{
    Vector2 aim_start = Vector2Subtract(player.aiming_at, Vector2Scale(PLAYER_AIMING_SIGHT_SIZE, 0.5));
    Vector2 aim_end = Vector2Add(aim_start, PLAYER_AIMING_SIGHT_SIZE);

    DrawLineEx(aim_start, aim_end, PLAYER_AIMING_SIGHT_THICKNESS, WHITE);
    DrawLineEx((Vector2){aim_start.x, aim_end.y}, (Vector2){aim_end.x, aim_start.y}, PLAYER_AIMING_SIGHT_THICKNESS, WHITE);
}

void player_draw(Player player)
{
    draw_entity(player.entity, locate_texture_spaceship(player.type));
    player_aiming_sight_draw(player);
}

Enemy *enemy_create(SpaceshipType type, Vector2 position, Vector2 movement_speed, f32 rotation)
{
    Enemy enemy = {.entity = {.position = position, .size = ENEMY_SIZE, .movement_speed = movement_speed, .rotation = rotation},
                   .type = type,
                   .shoot = ABILITY_SHOOT};

    return (Enemy *)data_object_pool_add(&state.enemies, &enemy);
}

void enemy_draw(Enemy enemy)
{
    DrawTexturePro(state.spritesheet, locate_texture_spaceship(enemy.type),
                   (Rectangle){enemy.entity.position.x, enemy.entity.position.y, enemy.entity.size.x, enemy.entity.size.y}, (Vector2){0},
                   enemy.entity.rotation, WHITE);
}
