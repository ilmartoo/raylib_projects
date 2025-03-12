#include "player_actions.h"
#include "inputs.h"
#include "rayheader.h"

f32 PlayerInputCheck(PlayerInput input, InputDevice device)
{
    if (device == INPUT_DEVICE_KEYBOARD_AND_MOUSE)
    {
        switch (input.method)
        {
        case INPUT_METHOD_MOUSE_BUTTON: return InputMouseButtonDown(input.value);
        case INPUT_METHOD_KEYBOARD_KEY: return InputKeyboardKeyDown(input.value);
        case INPUT_METHOD_POINTER_POSITION: return InputMouseCursorDelta(input.value);
        }
    }
    else if (device >= 0)
    {
        switch (input.method)
        {
        case INPUT_METHOD_GAMEPAD_BUTTON: return InputGamepadButtonDown(device, input.value);
        case INPUT_METHOD_GAMEPAD_TRIGGER: return InputGamepadTriggerPressure(device, input.value);
        case INPUT_METHOD_GAMEPAD_JOYSTICK: return InputGamepadJoystickOffset(device, input.value);
        }
    }
    return 0;
}