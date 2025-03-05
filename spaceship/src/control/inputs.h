#pragma once
#ifndef __SOURCE_INPUTS_H_
#define __SOURCE_INPUTS_H_

#include "raylib.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Definitions -----------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum Axis
{
    AXIS_X = GAMEPAD_AXIS_LEFT_X & GAMEPAD_AXIS_RIGHT_X, // 0b000
    AXIS_Y = GAMEPAD_AXIS_LEFT_Y & GAMEPAD_AXIS_RIGHT_Y, // 0b001
} Axis;

typedef enum Direction
{
    DIRECTION_POSITIVE = 0 << 2, // 0b000
    DIRECTION_NEGATIVE = 1 << 2, // 0b100
} Direction;

#define AXIS_DIRECTION_AXIS_FIELD      0b001
#define AXIS_DIRECTION_DIRECTION_FIELD 0b100

typedef enum AxisDirection
{
    AXIS_DIRECTION_POSITIVE_X = DIRECTION_POSITIVE | AXIS_X, // 0b000
    AXIS_DIRECTION_POSITIVE_Y = DIRECTION_POSITIVE | AXIS_Y, // 0b001
    AXIS_DIRECTION_NEGATIVE_X = DIRECTION_NEGATIVE | AXIS_X, // 0b100
    AXIS_DIRECTION_NEGATIVE_Y = DIRECTION_NEGATIVE | AXIS_Y, // 0b101
} AxisDirection;

typedef enum GamepadJoystick
{
    GAMEPAD_JOYSTICK_LEFT = GAMEPAD_AXIS_LEFT_X & GAMEPAD_AXIS_LEFT_Y,    // 0b000
    GAMEPAD_JOYSTICK_RIGHT = GAMEPAD_AXIS_RIGHT_X & GAMEPAD_AXIS_RIGHT_Y, // 0b010
} GamepadJoystick;

#define GAMEPAD_JOYSTICK_JOYSTICK_FIELD 0b010

typedef enum GamepadJoystickDirection
{
    GAMEPAD_JOYSTICK_LEFT_POSITIVE_X = GAMEPAD_JOYSTICK_LEFT | AXIS_DIRECTION_POSITIVE_X,   // 0b000
    GAMEPAD_JOYSTICK_LEFT_POSITIVE_Y = GAMEPAD_JOYSTICK_LEFT | AXIS_DIRECTION_POSITIVE_Y,   // 0b001
    GAMEPAD_JOYSTICK_RIGHT_POSITIVE_X = GAMEPAD_JOYSTICK_RIGHT | AXIS_DIRECTION_POSITIVE_X, // 0b010
    GAMEPAD_JOYSTICK_RIGHT_POSITIVE_Y = GAMEPAD_JOYSTICK_RIGHT | AXIS_DIRECTION_POSITIVE_Y, // 0b011
    GAMEPAD_JOYSTICK_LEFT_NEGATIVE_X = GAMEPAD_JOYSTICK_LEFT | AXIS_DIRECTION_NEGATIVE_X,   // 0b100
    GAMEPAD_JOYSTICK_LEFT_NEGATIVE_Y = GAMEPAD_JOYSTICK_LEFT | AXIS_DIRECTION_NEGATIVE_Y,   // 0b101
    GAMEPAD_JOYSTICK_RIGHT_NEGATIVE_X = GAMEPAD_JOYSTICK_RIGHT | AXIS_DIRECTION_NEGATIVE_X, // 0b110
    GAMEPAD_JOYSTICK_RIGHT_NEGATIVE_Y = GAMEPAD_JOYSTICK_RIGHT | AXIS_DIRECTION_NEGATIVE_Y, // 0b111
} GamepadJoystickDirection;

typedef enum GamepadTrigger
{
    GAMEPAD_TRIGGER_LEFT = GAMEPAD_AXIS_LEFT_TRIGGER,
    GAMEPAD_TRIGGER_RIGHT = GAMEPAD_AXIS_RIGHT_TRIGGER,
} GamepadTrigger;

// ----------------------------------------------------------------------------
// ---- Inputs ----------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum PlayerAction
{
    ACTION_MOVE_UP = 0,
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

#define PLAYER_ACTIONS_COUNT 11

typedef enum InputMethod
{
    INPUT_METHOD_NONE = 0,         // No input method
    INPUT_METHOD_MOUSE_BUTTON,     // Mouse button
    INPUT_METHOD_KEYBOARD_KEY,     // Keyboard key
    INPUT_METHOD_POINTER_POSITION, // Pointer position
    INPUT_METHOD_GAMEPAD_BUTTON,   // Gamepad button
    INPUT_METHOD_GAMEPAD_TRIGGER,  // Gamepad trigger
    INPUT_METHOD_GAMEPAD_JOYSTICK, // Gamepad joystick

} InputMethod;

typedef u16 InputValue;

typedef struct Input
{
    InputMethod method;
    InputValue value;
} Input;

typedef i16 InputDevice; // -2 for keyboard and mouse, -1 for no gamepad assigned and 0+ for gamepad IDs

typedef struct PlayerControls
{
    InputDevice device;
    Input actions[PLAYER_ACTIONS_COUNT];
} PlayerControls;

/**
 * Input functions, returning values between 0 (fully inactive) and 1 (fully active). The return values are set as such:
 * - `f32`: has inbetween values (returned as float value [0..1])
 * - `bool`: has only on and off values (returned as boolean value 0 or 1)
 */

bool mouse_button_down(MouseButton button);
bool keyboard_key_down(KeyboardKey key);
f32 pointer_position_offset(AxisDirection axis_direction, Vector2 origin, f32 max_difference);

bool gamepad_button_down(InputDevice gamepad_id, GamepadButton button);
f32 gamepad_trigger_pressure(InputDevice gamepad_id, GamepadTrigger trigger);
f32 gamepad_joystick_offset(InputDevice gamepad_id, GamepadJoystickDirection joystick_direction);

// ----------------------------------------------------------------------------
// ---- Defaults --------------------------------------------------------------
// ----------------------------------------------------------------------------

#define INPUT_DEVICE_KEYBOARD_AND_MOUSE (-2)
#define INPUT_DEVICE_NO_GAMEPAD         (-1)

#define DEFAULT_MOUSE_AND_KEYBOARD_INPUTS                                                                                                  \
    {                                                                                                                                      \
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

#define DEFAULT_GAMEPAD_INPUTS                                                                                                             \
    {                                                                                                                                      \
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

#endif // __SOURCE_INPUTS_H_