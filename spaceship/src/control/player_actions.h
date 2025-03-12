#pragma once
#ifndef __SOURCE_PLAYER_ACTIONS_H_
#define __SOURCE_PLAYER_ACTIONS_H_

#include "inputs.h"

// ----------------------------------------------------------------------------
// ---- Player Actions --------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum PlayerAction
{
    ACTION_PAUSE = 0,

    ACTION_MOVE_UP,
    ACTION_MOVE_RIGHT,
    ACTION_MOVE_DOWN,
    ACTION_MOVE_LEFT,

    ACTION_AIM_UP,
    ACTION_AIM_RIGHT,
    ACTION_AIM_DOWN,
    ACTION_AIM_LEFT,

    ACTION_ABILITY_SHOOT,
    ACTION_ABILITY_SHOOT_MISSILE,
    ACTION_ABILITY_DASH,
} PlayerAction;

#define PLAYER_ACTIONS_COUNT 12

typedef struct PlayerInput
{
    InputMethod method;
    u16 value;
} PlayerInput;

typedef struct PlayerControls
{
    InputDevice device;
    PlayerInput inputs[PLAYER_ACTIONS_COUNT];
} PlayerControls;

f32 PlayerInputCheck(PlayerInput input, InputDevice device, Vector2 origin, f32 max_distance);

// ----------------------------------------------------------------------------
// ---- Defaults --------------------------------------------------------------
// ----------------------------------------------------------------------------

#define PLAYER_INPUTS_DEFAULT_MOUSE_AND_KEYBOARD                                                                                           \
    {                                                                                                                                      \
        {INPUT_METHOD_KEYBOARD_KEY, KEY_P}, /* ACTION_PAUSE */                                                                             \
                                                                                                                                           \
        {INPUT_METHOD_KEYBOARD_KEY, KEY_W}, /* ACTION_MOVE_UP */                                                                           \
        {INPUT_METHOD_KEYBOARD_KEY, KEY_D}, /* ACTION_MOVE_RIGHT */                                                                        \
        {INPUT_METHOD_KEYBOARD_KEY, KEY_S}, /* ACTION_MOVE_DOWN */                                                                         \
        {INPUT_METHOD_KEYBOARD_KEY, KEY_A}, /* ACTION_MOVE_LEFT */                                                                         \
                                                                                                                                           \
        {INPUT_METHOD_POINTER_POSITION, AXIS_DIRECTION_NEGATIVE_Y}, /* ACTION_AIM_UP */                                                    \
        {INPUT_METHOD_POINTER_POSITION, AXIS_DIRECTION_POSITIVE_X}, /* ACTION_AIM_RIGHT */                                                 \
        {INPUT_METHOD_POINTER_POSITION, AXIS_DIRECTION_POSITIVE_Y}, /* ACTION_AIM_DOWN */                                                  \
        {INPUT_METHOD_POINTER_POSITION, AXIS_DIRECTION_NEGATIVE_X}, /* ACTION_AIM_LEFT */                                                  \
                                                                                                                                           \
        {INPUT_METHOD_MOUSE_BUTTON, MOUSE_BUTTON_LEFT},  /* ACTION_ABILITY_SHOOT */                                                        \
        {INPUT_METHOD_MOUSE_BUTTON, MOUSE_BUTTON_RIGHT}, /* ACTION_ABILITY_SHOOT_MISSILE */                                                \
        {INPUT_METHOD_KEYBOARD_KEY, KEY_SPACE},          /* ACTION_ABILITY_DASH */                                                         \
    }

#define PLAYER_INPUTS_DEFAULT_GAMEPAD                                                                                                      \
    {                                                                                                                                      \
        {INPUT_METHOD_GAMEPAD_BUTTON, GAMEPAD_BUTTON_MIDDLE_RIGHT}, /* ACTION_PAUSE */                                                     \
                                                                                                                                           \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_NEGATIVE_Y}, /* ACTION_MOVE_UP */                                            \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_POSITIVE_X}, /* ACTION_MOVE_RIGHT */                                         \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_POSITIVE_Y}, /* ACTION_MOVE_DOWN */                                          \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_NEGATIVE_X}, /* ACTION_MOVE_LEFT */                                          \
                                                                                                                                           \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_NEGATIVE_Y}, /* ACTION_AIM_UP */                                            \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_POSITIVE_X}, /* ACTION_AIM_RIGHT */                                         \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_POSITIVE_Y}, /* ACTION_AIM_DOWN */                                          \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_NEGATIVE_X}, /* ACTION_AIM_LEFT */                                          \
                                                                                                                                           \
        {INPUT_METHOD_GAMEPAD_TRIGGER, GAMEPAD_TRIGGER_RIGHT},        /* ACTION_ABILITY_SHOOT */                                           \
        {INPUT_METHOD_GAMEPAD_TRIGGER, GAMEPAD_TRIGGER_LEFT},         /* ACTION_ABILITY_SHOOT_MISSILE */                                   \
        {INPUT_METHOD_GAMEPAD_BUTTON, GAMEPAD_BUTTON_LEFT_TRIGGER_1}, /* ACTION_ABILITY_DASH */                                            \
    }

#endif // __SOURCE_PLAYER_ACTIONS_H_