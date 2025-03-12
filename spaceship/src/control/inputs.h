#pragma once
#ifndef __SOURCE_INPUTS_H_
#define __SOURCE_INPUTS_H_

#include "rayheader.h"

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

typedef i16 InputDevice; // -2 for keyboard and mouse, -1 for no gamepad assigned and 0+ for gamepad IDs

#define INPUT_DEVICE_KEYBOARD_AND_MOUSE -2
#define INPUT_DEVICE_NO_GAMEPAD         -1

bool InputKeyboardKeyDown(KeyboardKey key);        // [0, 1] Discrete value
bool InputMouseButtonDown(MouseButton button);     // [0, 1] Discrete value
f32 InputMouseCursorDelta(AxisDirection axis_dir); // [0..Inf] Continuous value (Cursor pos increase from last frame in that direction)

bool InputGamepadButtonDown(InputDevice gamepad_id, GamepadButton button);                     // [0, 1] Discrete value
f32 InputGamepadTriggerPressure(InputDevice gamepad_id, GamepadTrigger trigger);               // [0..1] Continuous value
f32 InputGamepadJoystickOffset(InputDevice gamepad_id, GamepadJoystickDirection joystick_dir); // [0..1] Continuous value

#endif // __SOURCE_INPUTS_H_