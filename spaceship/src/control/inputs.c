#include "inputs.h"
#include "debug.h"
#include "rayheader.h"

bool mouse_button_down(MouseButton button) { return IsMouseButtonDown(button); }

bool keyboard_key_down(KeyboardKey key) { return IsKeyDown(key); }

f32 pointer_position_offset(AxisDirection axis_direction, Vector2 origin, f32 max_difference)
{
    Axis axis = axis_direction & AXIS_DIRECTION_AXIS_FIELD;
    Direction direction = axis_direction & AXIS_DIRECTION_DIRECTION_FIELD;

    Vector2 mouse_position = GetMousePosition();
    Vector2 offset_vector = Vector2Subtract(mouse_position, origin);

    Vector2 magnitude_vector = Vector2Normalize(offset_vector);

    f32 length = Vector2Length(offset_vector);
    if (length < max_difference) { magnitude_vector = Vector2Scale(magnitude_vector, length / max_difference); }

    f32 axis_magnitude = axis == AXIS_X ? magnitude_vector.x : magnitude_vector.y;
    f32 magnitude = direction == DIRECTION_POSITIVE ? max(0.f, axis_magnitude) : -min(0.f, axis_magnitude);
    return minmax(magnitude, 0, 1);
}

bool gamepad_button_down(InputDevice gamepad, GamepadButton button) { return IsGamepadButtonDown(gamepad, button); }

f32 gamepad_trigger_pressure(InputDevice gamepad, GamepadTrigger trigger)
{
    f32 magnitude = GetGamepadAxisMovement(gamepad, trigger); // -1 to 1
    magnitude = (magnitude + 1) * 0.5;
    return minmax(magnitude, 0, 1);
}

f32 gamepad_joystick_offset(InputDevice gamepad, GamepadJoystickDirection joystick_direction)
{
    Axis axis = joystick_direction & AXIS_DIRECTION_AXIS_FIELD;
    Direction direction = joystick_direction & AXIS_DIRECTION_DIRECTION_FIELD;
    Direction joystick = joystick_direction & GAMEPAD_JOYSTICK_JOYSTICK_FIELD;

    f32 joystick_movement = GetGamepadAxisMovement(gamepad, joystick | axis);
    f32 magnitude = direction == DIRECTION_POSITIVE ? max(0.f, joystick_movement) : -min(0.f, joystick_movement);
    return minmax(magnitude, 0, 1);
}
