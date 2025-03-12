#include <math.h>

#include "inputs.h"
#include "debug.h"
#include "rayheader.h"

bool InputKeyboardKeyDown(KeyboardKey key) { return IsKeyDown(key); }

bool InputMouseButtonDown(MouseButton button) { return IsMouseButtonDown(button); }

f32 InputMouseCursorDelta(AxisDirection axis_direction)
{
    Axis axis = axis_direction & AXIS_DIRECTION_AXIS_FIELD;
    Direction direction = axis_direction & AXIS_DIRECTION_DIRECTION_FIELD;

    Vector2 mouse_delta = GetMouseDelta();
    f32 axis_magnitude = axis == AXIS_X ? mouse_delta.x : mouse_delta.y;
    return direction == DIRECTION_POSITIVE ? max(0, axis_magnitude) : -min(0, axis_magnitude);
}

// f32 InputCursorOffset(AxisDirection axis_direction, Vector2 origin, f32 max_difference)
// {
//     Axis axis = axis_direction & AXIS_DIRECTION_AXIS_FIELD;
//     Direction direction = axis_direction & AXIS_DIRECTION_DIRECTION_FIELD;
//
//     Vector2 mouse_position = GetMousePosition();
//     Vector2 offset_vector = Vector2Subtract(mouse_position, origin);
//
//     Vector2 magnitude_vector;
//     if (max_difference <= 0) { magnitude_vector = Vector2Divide(offset_vector, Vector2From(fabs(offset_vector.x),
//     fabs(offset_vector.y))); } else
//     {
//         magnitude_vector = Vector2Normalize(offset_vector);
//
//         f32 length = Vector2Length(offset_vector);
//         if (length < max_difference) { magnitude_vector = Vector2Scale(magnitude_vector, length / max_difference); }
//     }
//
//     f32 axis_magnitude = axis == AXIS_X ? magnitude_vector.x : magnitude_vector.y;
//     return minmax(magnitude, 0, 1); mouse_delta.x : mouse_delta.y;
//     f32 magnitude = direction == DIRECTION_POSITIVE ? max(0, axis_magnitude) : -min(0, axis_magnitude);
//     return minmax(magnitude, 0, 1);
// }

bool InputGamepadButtonDown(InputDevice gamepad, GamepadButton button) { return IsGamepadButtonDown(gamepad, button); }

f32 InputGamepadTriggerPressure(InputDevice gamepad, GamepadTrigger trigger)
{
    f32 magnitude = GetGamepadAxisMovement(gamepad, trigger); // -1 to 1
    return (magnitude + 1) * 0.5f;
}

f32 InputGamepadJoystickOffset(InputDevice gamepad, GamepadJoystickDirection joystick_direction)
{
    Axis axis = joystick_direction & AXIS_DIRECTION_AXIS_FIELD;
    Direction direction = joystick_direction & AXIS_DIRECTION_DIRECTION_FIELD;
    GamepadJoystick joystick = joystick_direction & GAMEPAD_JOYSTICK_JOYSTICK_FIELD;

    f32 joystick_movement = GetGamepadAxisMovement(gamepad, joystick | axis);
    return direction == DIRECTION_POSITIVE ? max(0, joystick_movement) : -min(0, joystick_movement);
}
