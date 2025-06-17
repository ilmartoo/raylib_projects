#include "input-handler.h"

#include "rayconfig.h"
#include "raylib.h"
#include "raymath.h"
#include "types.h"

typedef struct {
    InputResult keyboard_keys[MAX_KEYBOARD_KEYS];
    InputResult mouse_buttons[MAX_MOUSE_BUTTONS];
    InputResult keyboard_keys[MAX_KEYBOARD_KEYS];
} InputCache;

InputCache caches[2] = {0};

// Check if the input method is valid for the specified input device
bool IsValidInputMethod(InputDeviceID device, InputMethod method) {
    // Gamepad check
    if (device >= 0) {
        return !IS_INPUT_METHOD_FROM_KEYBOARD_AND_MOUSE(method);
    }
    // Keyboard & Mouse check
    else if (device == INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE) {
        return IS_INPUT_METHOD_FROM_KEYBOARD_AND_MOUSE(method);
    }
    // No input method
    else if (device == INPUT_DEVICE_ID_NULL) {
        return method == INPUT_METHOD_NONE;
    }
    // Other configuration
    return false;
}

// Set an input map - action_id must be a valid index for the actions array
bool InputHandlerSet(InputDeviceID device, InputMap* mappings, InputActionID action_id, const InputMap map) {
    if (IsValidInputMethod(device, map.method)) {
        mappings[action_id] = map;
        return true;
    }
    return false;
}

InputResult InputHandlerGetValue(InputDeviceID device, const InputMap mappings[], InputActionID action_id) {
    InputMap map = mappings[action_id];
    switch (map.method) {
        // Keyboard Key - bool
        case INPUT_METHOD_KEYBOARD_KEY: {
            bool value = IsKeyDown(map.input_id);
            return (InputResult){.bool_v = value, .float_v = value};
        }

        // Mouse Button - bool
        case INPUT_METHOD_MOUSE_BUTTON: {
            bool value = IsMouseButtonDown(map.input_id);
            return (InputResult){.bool_v = value, .float_v = value};
        }

        // Mouse Position - float
        case INPUT_METHOD_MOUSE_POSITION: {
            Vector2 position = GetMousePosition();
            Vector2 delta = GetMouseDelta();
            return map.input_id == MOUSE_AXIS_X ? (InputResult){.float_v = position.x, .bool_v = !FloatEquals(0, delta.x)}
                                                : (InputResult){.float_v = position.y, .bool_v = !FloatEquals(0, delta.y)};
        }

        // Mouse Movement - float
        case INPUT_METHOD_MOUSE_MOVEMENT: {
            Vector2 delta = GetMouseDelta();
            return map.input_id == MOUSE_AXIS_X ? (InputResult){.float_v = delta.x, .bool_v = !FloatEquals(0, delta.x)}
                                                : (InputResult){.float_v = delta.y, .bool_v = !FloatEquals(0, delta.y)};
        }

        // Mouse Scroll - float
        case INPUT_METHOD_MOUSE_SCROLL: {
            f32 value = GetMouseWheelMove();
            return (InputResult){.float_v = value, .bool_v = !FloatEquals(0, value)};
        }

        // Gamepad Button - bool
        case INPUT_METHOD_GAMEPAD_BUTTON: {
            bool value = IsGamepadButtonDown(device, map.input_id);
            return (InputResult){.bool_v = value, .float_v = value};
        }

        // Gamepad Trigger (normalized or not) - float
        case INPUT_METHOD_GAMEPAD_TRIGGER: {
            f32 value = GetGamepadAxisMovement(device, map.input_id | GAMEPAD_AXIS_LEFT_TRIGGER);
            return (InputResult){.float_v = value, .bool_v = !FloatEquals(-1.f, value)};
        }

        // Gamepad Joystick - float
        case INPUT_METHOD_GAMEPAD_JOYSTICK: {
            input_size input_id = INPUT_METHOD_GAMEPAD_JOYSTICK_GET_JOYSTICK(map.input_id);  // Retrieve the input id value
            u16 death_zone = INPUT_METHOD_GAMEPAD_JOYSTICK_GET_DEATH_ZONE(map.input_id);     // Retrieve the death zone value
            f32 value = GetGamepadAxisMovement(device, input_id);
            return (fabsf(value) * INPUT_METHOD_GAMEPAD_JOYSTICK_DEATH_ZONE_PRECISION) > death_zone
                       ? (InputResult){.float_v = value, .bool_v = true}
                       : (InputResult){.float_v = 0, .bool_v = false};
        }

        // Gamepad Trigger (normalized) - float - from `-1..1` to `0..1`
        case INPUT_METHOD_GAMEPAD_TRIGGER_NORM: {
            f32 value = (GetGamepadAxisMovement(device, map.input_id) + 1.0f) * 0.5f;
            return (InputResult){.float_v = value, .bool_v = !FloatEquals(0, value)};
        }

        // No input method - false
        default: return (InputResult){.bool_v = false, .float_v = 0};
    }
}

// ----------------------------------------------------------------------------
// ---- Basic Input Handler ---------------------------------------------------
// ----------------------------------------------------------------------------

BasicInputHandler BasicInputHandlerCreate(InputDeviceID device, input_size n_actions) {
    return (BasicInputHandler){
        .device = device,
        .mappings = reserve_array_zero(InputMap, n_actions),
        .size = n_actions,
    };
}

void BasicInputHandlerDelete(BasicInputHandler* handler) { free(handler->mappings); }

bool BasicInputHandlerMapSet(BasicInputHandler* handler, InputActionID action_id, const InputMap map) {
    return InputHandlerSet(handler->device, handler->mappings, action_id, map);
}

u16 BasicInputHandlerMappingsSet(BasicInputHandler* handler, const InputMap mappings[]) {
    u16 errors = 0;
    for (u16 i = 0; i < handler->size; ++i) { errors += InputHandlerSet(handler->device, handler->mappings, i, mappings[i]); }
    return errors;
}

InputResult BasicInputHandlerGetValue(BasicInputHandler handler, InputActionID action_id) {
    return InputHandlerGetValue(handler.device, handler.mappings, action_id);
}

// ----------------------------------------------------------------------------
// ---- Greedy Input Handler --------------------------------------------------
// ----------------------------------------------------------------------------

GreedyInputHandler GreedyInputHandlerCreate(input_size n_actions) {
    byte* buffer = (byte*)malloc((sizeof(InputMap) * n_actions * 2) + (sizeof(InputResult) * n_actions));
    return (GreedyInputHandler){
        .keyboard_mouse_mappings = (InputMap*)buffer,
        .gamepad_mappings = (InputMap*)&buffer[sizeof(InputMap) * n_actions],
        .results = (InputResult*)&buffer[sizeof(InputMap) * n_actions * 2],
        .size = n_actions,
        .active_device = INPUT_DEVICE_ID_DEFAULT,
        .active_device_state = INPUT_DEVICE_STATE_INITIAL,
    };
}

void GreedyInputHandlerDelete(GreedyInputHandler* handler) { free(handler->keyboard_mouse_mappings); }

bool GreedyInputHandlerMapSet(GreedyInputHandler* handler, InputDeviceID device, InputActionID action_id, const InputMap map) {
    if (device >= 0) {
        return InputHandlerSet(INPUT_DEVICE_ID_FIRST_GAMEPAD, handler->gamepad_mappings, handler->size, map);
    } else if (device == INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE) {
        return InputHandlerSet(INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE, handler->keyboard_mouse_mappings, handler->size, map);
    }
    return false;
}

u16 GreedyInputHandlerDeviceMappingsSet(GreedyInputHandler* handler, InputDeviceID device, const InputMap mappings[]) {
    u16 errors = 0;

    InputMap* current_mappings =
        device >= 0 ? handler->gamepad_mappings : (device == INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE ? handler->keyboard_mouse_mappings : NULL);

    if (current_mappings != NULL) {
        for (u16 i = 0; i < handler->size; ++i) { errors += InputHandlerSet(device, current_mappings, i, mappings[i]); }
    } else {
        errors += handler->size;
    }

    return errors;
}

u16 GreedyInputHandlerMappingsSet(GreedyInputHandler* handler,
                                  const InputMap keyboard_mouse_mappings[],
                                  const InputMap gamepad_mappings[]) {
    u16 errors = 0;

    for (u16 i = 0; i < handler->size; ++i) {
        errors += InputHandlerSet(INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE, handler->keyboard_mouse_mappings, i, keyboard_mouse_mappings[i]);
    }

    for (u16 i = 0; i < handler->size; ++i) {
        errors += InputHandlerSet(INPUT_DEVICE_ID_FIRST_GAMEPAD, handler->gamepad_mappings, i, gamepad_mappings[i]);
    }

    return errors;
}

// Returns `true` if an input had a boolean value different from `0` (representing the device being the one used)
bool GreedyInputHandlerUpdateResultsWithDevice(GreedyInputHandler* handler, InputDeviceID device) {
    bool is_being_used = false;
    InputMap* mappings = NULL;

    // Special devices
    if (device < 0) {
        if (device == INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE) { mappings = handler->keyboard_mouse_mappings; }
    }
    // Gamepads
    else {
        if (IsGamepadAvailable(device)) { mappings = handler->gamepad_mappings; }
    }

    if (mappings != NULL) {
        for (InputActionID action_id = 0; action_id < handler->size; ++action_id) {
            is_being_used = (handler->results[action_id] = InputHandlerGetValue(device, mappings, action_id)).bool_v || is_being_used;
        }
    }
    return is_being_used;
}

InputDeviceResults GreedyInputHandlerUpdate(GreedyInputHandler* handler) {
    bool active_device_missing = false;
    bool active_device_used = false;

    // Current device missing check
    if (handler->active_device >= 0 && !IsGamepadAvailable(handler->active_device)) {
        active_device_missing = true;
        handler->active_device = INPUT_DEVICE_ID_DEFAULT;  // Set default input device in case no other device is used
    }
    // Current device usage check
    else if (GreedyInputHandlerUpdateResultsWithDevice(handler, handler->active_device)) {
        active_device_used = true;
    }

    if (active_device_missing || !active_device_used) {
        // Gamepad usage check
        for (InputDeviceID device = 0; device < MAX_GAMEPADS; ++device) {
            if (device != handler->active_device && GreedyInputHandlerUpdateResultsWithDevice(handler, device)) {
                handler->active_device = device;
                handler->active_device_state =
                    active_device_missing ? INPUT_DEVICE_STATE_CHANGE_MISSING_TO_ACTIVE  // Last active device went missing - Change
                                          : INPUT_DEVICE_STATE_CHANGE_IDLE_TO_ACTIVE;    // Last active device was idle - Change

                return (InputDeviceResults){
                    .device = handler->active_device,
                    .results = handler->results,
                };
            }
        }

        // K&M usage check
        if (GreedyInputHandlerUpdateResultsWithDevice(handler, INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE)) {
            handler->active_device = INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE;
            handler->active_device_state = active_device_missing
                                               ? INPUT_DEVICE_STATE_CHANGE_MISSING_TO_ACTIVE  // Last active device went missing - Change
                                               : INPUT_DEVICE_STATE_CHANGE_IDLE_TO_ACTIVE;    // Last active device was idle - Change

            return (InputDeviceResults){
                .device = handler->active_device,
                .results = handler->results,
            };
        }
    }

    handler->active_device_state = active_device_missing
                                       ? INPUT_DEVICE_STATE_CHANGE_MISSING_TO_DEFAULT     // Active device went missing - Default selected
                                       : (active_device_used ? INPUT_DEVICE_STATE_ACTIVE  // Active device in use - No change
                                                             : INPUT_DEVICE_STATE_IDLE);  // Active device is idle - No change

    return (InputDeviceResults){
        .device = handler->active_device,
        .results = handler->results,
    };
}

InputResult GreedyInputHandlerGetValue(GreedyInputHandler handler, InputActionID action_id) { return handler.results[action_id]; }

InputResult* GreedyInputHandlerGetAllValues(GreedyInputHandler handler) { return handler.results; }
