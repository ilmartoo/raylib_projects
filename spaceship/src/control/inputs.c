#include <math.h>

#include "debug.h"
#include "inputs.h"
#include "raylib.h"
#include "raymath.h"

Axis InputGetAxis(u8 input_with_axis) { return input_with_axis & INPUT_AXIS_FIELD; }

GamepadJoystick InputGetJoystick(u8 input_with_joystick) { return input_with_joystick & INPUT_JOYSTICK_FIELD; }

Direction InputGetDirection(u8 input_with_direction) { return input_with_direction & INPUT_DIRECTION_FIELD; }

bool IsInputKeyboardKeyDown(KeyboardKey key) { return IsKeyDown(key); }

bool IsInputMouseButtonDown(MouseButton button) { return IsMouseButtonDown(button); }

f32 InputMouseCursorPosition(AxisDirection axis_direction, Vector2 origin)
{
    Axis axis = InputGetAxis(axis_direction);
    Direction direction = InputGetDirection(axis_direction);

    Vector2 offset_vector = Vector2Subtract(GetMousePosition(), origin);
    Vector2 magnitude_vector = Vector2Scale(offset_vector, 1.f / max(fabs(offset_vector.x), fabs(offset_vector.y)));

    f32 axis_magnitude = axis == AXIS_X ? magnitude_vector.x : magnitude_vector.y;
    f32 magnitude = direction == DIRECTION_POSITIVE ? max(0, axis_magnitude) : -min(0, axis_magnitude);
    return minmax(magnitude, 0, 1);
}

bool IsInputGamepadButtonDown(InputDevice gamepad, GamepadButton button) { return IsGamepadButtonDown(gamepad, button); }

f32 InputGamepadTriggerPressure(InputDevice gamepad, GamepadTrigger trigger)
{
    f32 magnitude = GetGamepadAxisMovement(gamepad, trigger); // -1 to 1
    return (magnitude + 1) * 0.5f;
}

f32 InputGamepadJoystickOffset(InputDevice gamepad, GamepadJoystickDirection joystick_direction)
{
    Axis axis = InputGetAxis(joystick_direction);
    GamepadJoystick joystick = InputGetJoystick(joystick_direction);
    Direction direction = InputGetDirection(joystick_direction);

    f32 joystick_movement = GetGamepadAxisMovement(gamepad, joystick | axis);
    return direction == DIRECTION_POSITIVE ? max(0, joystick_movement) : -min(0, joystick_movement);
}
