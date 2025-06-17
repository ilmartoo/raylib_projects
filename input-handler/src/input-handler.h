#pragma once
#ifndef INPUTS_H
#define INPUTS_H

#include "rayconfig.h"
#include "raylib.h"
#include "types.h"

// ----------------------------------------------------------------------------
// ---- Raylib Configurations -------------------------------------------------
// ----------------------------------------------------------------------------

#ifndef MAX_KEYBOARD_KEYS
#define MAX_KEYBOARD_KEYS 512  // Maximum number of keyboard keys supported
#endif

#ifndef MAX_MOUSE_BUTTONS
#define MAX_MOUSE_BUTTONS 8  // Maximum number of mouse buttons supported
#endif

#ifndef MAX_GAMEPADS
#define MAX_GAMEPADS 4  // Maximum number of gamepads supported
#endif

#ifndef MAX_GAMEPAD_AXIS
#define MAX_GAMEPAD_AXIS 6  // Maximum number of axis supported (per gamepad) (2 axis per joystick + 2 triggers)
#endif

#ifndef MAX_GAMEPAD_BUTTONS
#define MAX_GAMEPAD_BUTTONS 17  // Maximum number of buttons supported (per gamepad)
#endif

// ----------------------------------------------------------------------------
// ---- Input Devices and Methods ---------------------------------------------
// ----------------------------------------------------------------------------

// Device ID of the input device to capture the input from:
// - __Positive number__: Gamepad input device. From `0` to `MAX_GAMEPADS - 1`.
// - __Negative number__: Reserved for special devices. Keyboard & Mouse being one of them.
typedef i16 InputDeviceID;

#define INPUT_DEVICE_ID_NULL               -2                                  // Device ID for no device
#define INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE -1                                  // Device ID of keyboard and mouse
#define INPUT_DEVICE_ID_FIRST_GAMEPAD      0                                   // Device ID of the first gamepad
#define INPUT_DEVICE_ID_LAST_GAMEPAD       (MAX_GAMEPADS - 1)                  // Device ID of the last gamepad
#define INPUT_DEVICE_ID_DEFAULT            INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE  // Default device ID

// Information about the state of an input device
typedef enum {
    // clang-format off
    INPUT_DEVICE_STATE_INITIAL = 0,                // The input handler had just been initialized so the default device is selected
    INPUT_DEVICE_STATE_ACTIVE,                     // The active device is being used
    INPUT_DEVICE_STATE_IDLE,                       // The active device is idle and no other device is being used
    INPUT_DEVICE_STATE_MISSING,                    // The active device has gone missing
    INPUT_DEVICE_STATE_CHANGE_IDLE_TO_ACTIVE,      // The active device was changed because it was idle and the current device was being used
    INPUT_DEVICE_STATE_CHANGE_MISSING_TO_ACTIVE,   // The active device was changed because it went missing and the current device was being used
    INPUT_DEVICE_STATE_CHANGE_MISSING_TO_DEFAULT,  // The active device was changed because it went missing, so the default device was selected
    // clang-format on
} InputDeviceState;

// Method of input to associate an input id to the correct key, mouse position, button, trigger or joystick:
// - Keyboard & Mouse input method - Input method value has 1 in its least significant bit
// - Gamepad input method          - Input method value has 0 in its in least significant bit
typedef enum {
    // No input method
    INPUT_METHOD_NONE = 0,

    // KEYBOARD KEYS //

    // Activates when a specified keyboard key is pressed (from UP position to DOWN position)
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_KEY_PRESSED = 1,
    // Activates when a specified keyboard key is released (from DOWN position to UP position)
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_KEY_RELEASED = 3,
    // Activates when a specified keyboard key is down
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_KEY_DOWN = 5,
    // Activates when a specified keyboard key is up
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_KEY_UP = 7,
    // Activates when any keyboard key is pressed (from UP position to DOWN position)
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_ANY_KEY_PRESSED = 9,
    // Activates when any keyboard key is released (from DOWN position to UP position)
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_KEY_RELEASED = 11,
    // Activates when any keyboard key is down
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_ANY_KEY_DOWN = 13,
    // Activates when any keyboard key is up
    // - Values `0, 1`
    INPUT_METHOD_KEYBOARD_ANY_KEY_UP = 15,

    // MOUSE BUTTONS //

    // Activates when a specified mouse button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_BUTTON_PRESSED = 17,
    // Activates when a specified mouse button is pressed (from DOWN position to UP position)
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_BUTTON_RELEASED = 19,
    // Activates when a specified mouse button is down
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_BUTTON_DOWN = 21,
    // Activates when a specified mouse button is up
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_BUTTON_UP = 23,
    // Activates when any mouse button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_ANY_BUTTON_PRESSED = 25,
    // Activates when any mouse button is pressed (from DOWN position to UP position)
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_ANY_BUTTON_RELEASED = 27,
    // Activates when any mouse button is down
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_ANY_BUTTON_DOWN = 29,
    // Activates when any mouse button is up
    // - Values: `0, 1`
    INPUT_METHOD_MOUSE_ANY_BUTTON_UP = 31,

    // MOUSE MOVEMENTS //

    // Activates when the mouse moves with its current position
    // - Values: `FLOAT_MIN..FLOAT_MAX`
    // - Movement threshold can be encoded
    INPUT_METHOD_MOUSE_POSITION = 33,
    // Activates when the mouse moves with the frame delta movement
    // - Values: `FLOAT_MIN..FLOAT_MAX`
    // - Movement threshold can be encoded
    INPUT_METHOD_MOUSE_MOVEMENT = 35,
    // Activates when the mouse scrolls with the scrolled distance
    // - Values: `FLOAT_MIN..FLOAT_MAX`
    // - Scroll threshold can be encoded
    INPUT_METHOD_MOUSE_SCROLL = 37,

    // GAMEPAD BUTTONS //

    // Activates when a specified gamepad button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    INPUT_METHOD_GAMEPAD_BUTTON_PRESSED = 2,
    // Activates when a specified gamepad button is released (from DOWN position to UP position)
    // - Values `0, 1`
    INPUT_METHOD_GAMEPAD_BUTTON_RELEASED = 4,
    // Activates when a specified gamepad button is down
    // - Values `0, 1`
    INPUT_METHOD_GAMEPAD_BUTTON_DOWN = 6,
    // Activates when a specified gamepad button is up
    // - Values `0, 1`
    INPUT_METHOD_GAMEPAD_BUTTON_UP = 8,
    // Activates when any gamepad button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    INPUT_METHOD_GAMEPAD_ANY_BUTTON_PRESSED = 10,
    // Activates when any gamepad button is pressed (from DOWN position to UP position)
    // - Values: `0, 1`
    INPUT_METHOD_GAMEPAD_ANY_BUTTON_RELEASED = 12,
    // Activates when any gamepad button is down
    // - Values: `0, 1`
    INPUT_METHOD_GAMEPAD_ANY_BUTTON_DOWN = 14,
    // Activates when any gamepad button is up
    // - Values: `0, 1`
    INPUT_METHOD_GAMEPAD_ANY_BUTTON_UP = 16,

    // GAMEPAD TRIGGERS //

    // Activates when a specified gamepad trigger is pressed
    // - Values: `-1..1`
    // - Death zone threshold can be encoded
    INPUT_METHOD_GAMEPAD_TRIGGER = 18,
    // Activates when a specified gamepad trigger is pressed (value normalized)
    // - Values: `0..1`
    // - Death zone threshold can be encoded (applied before normalization)
    INPUT_METHOD_GAMEPAD_TRIGGER_NORMALIZED = 20,

    // GAMEPAD JOYSTICKS //

    // Activates when a specified gamepad joystick is moved along the specified axis
    // - Values: `-1..1`
    // - Death zone threshold can be encoded
    INPUT_METHOD_GAMEPAD_JOYSTICK = 22,

} InputMethod;

#define IS_INPUT_METHOD_FROM_KEYBOARD_AND_MOUSE(method) (method & 0b1)  // Check if is a valid keyboard & mouse input method

// Gamepad Trigger input
typedef enum {
    GAMEPAD_TRIGGER_LEFT = GAMEPAD_AXIS_LEFT_TRIGGER & 0b1,
    GAMEPAD_TRIGGER_RIGHT = GAMEPAD_AXIS_RIGHT_TRIGGER & 0b1,
} GamepadTrigger;

// Gamepad Joysticks input
typedef enum {
    GAMEPAD_JOYSTICK_LEFT_X = GAMEPAD_AXIS_LEFT_X,
    GAMEPAD_JOYSTICK_LEFT_Y = GAMEPAD_AXIS_LEFT_Y,
    GAMEPAD_JOYSTICK_RIGHT_X = GAMEPAD_AXIS_RIGHT_X,
    GAMEPAD_JOYSTICK_RIGHT_Y = GAMEPAD_AXIS_RIGHT_Y,
} GamepadJoystick;

// Mouse Axis input (Position and Movement)
typedef enum {
    MOUSE_AXIS_X = GAMEPAD_AXIS_LEFT_X & GAMEPAD_AXIS_RIGHT_X,
    MOUSE_AXIS_Y = GAMEPAD_AXIS_LEFT_Y & GAMEPAD_AXIS_RIGHT_Y,
} MouseAxis;

// Mouse Scroll input
typedef enum {
    MOUSE_SCROLL_WHEEL = 0,
} MouseScroll;

// ----------------------------------------------------------------------------
// ---- Input Actions, Results and Mappings -----------------------------------
// ----------------------------------------------------------------------------

// Type for the maximum size of the actions array
typedef u16 input_size;

// An input action ID - The action IDs valid range from `0` to `2^16 - 1`
typedef input_size InputActionID;

// An input mapping. Might be one of the following configurations, depending on the device selected at the handler:
//
// Keyboard and mouse:
// - `{ method: INPUT_METHOD_KEYBOARD_KEY, input_id: KeyboardKey }`
// - `{ method: INPUT_METHOD_MOUSE_BUTTON, input_id: MouseButton }`
// - `{ method: INPUT_METHOD_MOUSE_POSITION, input_id: MouseAxis }`
// - `{ method: INPUT_METHOD_MOUSE_MOVEMENT, input_id: MouseAxis }`
// - `{ method: INPUT_METHOD_MOUSE_SCROLL, input_id: MouseScroll }`
//
// Gamepad:
// - `{ method: INPUT_METHOD_GAMEPAD_BUTTON, input_id: GamepadButton }`
// - `{ method: INPUT_METHOD_GAMEPAD_TRIGGER, input_id: GamepadTrigger }` *
// - `{ method: INPUT_METHOD_GAMEPAD_TRIGGER_NORMALIZED, input_id: GamepadTrigger }` *
// - `{ method: INPUT_METHOD_GAMEPAD_JOYSTICK, input_id: GamepadJoystick }` *
//
// `*`
// The input id has a death zone encoded inside the id. The first `INPUT_METHOD_ID_N_BITS` are the corresponding input id,
// while the rest are the death zone
typedef struct {
    InputMethod method;
    input_size input_id;
} InputMap;

#define INPUT_METHOD_GAMEPAD_ID_BITS               2      // The number of bits containing the input id
#define INPUT_METHOD_GAMEPAD_PAYLOAD_PRESENCE_BITS 1      // The number of bits that indicates if a payload is present in the input id
#define INPUT_METHOD_GAMEPAD_ID_MASK               0b011  // The bit mask of the bits containing the input id
#define INPUT_METHOD_GAMEPAD_PAYLOAD_PRESENCE_MASK 0b100  // The bit mask of the bits containing the playload presence value

#define _PRECISION_GENERATOR(precision) 1E##precision            // A precission multiplicator generator
#define INPUT_METHOD_DEATH_ZONE_MULT    _PRECISION_GENERATOR(3)  // The multiplier used with the death zone to retrieve de significant decimals

// clang-format off
#define INPUT_METHOD_GAMEPAD_JOYSTICK_INPUT_ID(joystick, death_zone) ((death_zone << INPUT_METHOD_GAMEPAD_JOYSTICK_ID_N_BITS) | joystick)  // Creates a gamepad joystick input id from a joystick and a death zone
#define INPUT_METHOD_GAMEPAD_JOYSTICK_GET_JOYSTICK(input_id)         (input_id & INPUT_METHOD_GAMEPAD_JOYSTICK_DEATH_ZONE_BITS)            // Retrieves the joystick value from a gamepad joystick input id
#define INPUT_METHOD_GAMEPAD_JOYSTICK_GET_DEATH_ZONE(input_id)       (input_id >> INPUT_METHOD_GAMEPAD_JOYSTICK_ID_N_BITS)                 // Retrieves the death zone value from a gamepad joystick input id

#define _INPUT_MAP(method, id) ((InputMap){method, id})  // Input map creator helper

#define INPUT_MAP_NONE                                   _INPUT_MAP(INPUT_METHOD_NONE, 0)                                                                         // No mapping
#define INPUT_MAP_KEYBOARD_KEY(key)                      _INPUT_MAP(INPUT_METHOD_KEYBOARD_KEY, key)                                                               // Mapping of a keyboard key
#define INPUT_MAP_MOUSE_BUTTON(button)                   _INPUT_MAP(INPUT_METHOD_MOUSE_BUTTON, button)                                                            // Mapping of a mouse button press
#define INPUT_MAP_MOUSE_POSITION(mouse_axis)             _INPUT_MAP(INPUT_METHOD_MOUSE_POSITION, mouse_axis)                                                      // Mapping of a mouse position change
#define INPUT_MAP_MOUSE_MOVEMENT(mouse_axis)             _INPUT_MAP(INPUT_METHOD_MOUSE_MOVEMENT, mouse_axis)                                                      // Mapping of a mouse movement delta
#define INPUT_MAP_MOUSE_SCROLL(scroll)                   _INPUT_MAP(INPUT_METHOD_MOUSE_SCROLL, scroll)                                                            // Mapping of a mouse scroll
#define INPUT_MAP_GAMEPAD_BUTTON(button)                 _INPUT_MAP(INPUT_METHOD_GAMEPAD_BUTTON, button)                                                          // Mapping of a gamepad button press
#define INPUT_MAP_GAMEPAD_TRIGGER(trigger)               _INPUT_MAP(INPUT_METHOD_GAMEPAD_TRIGGER, trigger)                                                        // Mapping of a trigger position
#define INPUT_MAP_GAMEPAD_TRIGGER_NORM(trigger)          _INPUT_MAP(INPUT_METHOD_GAMEPAD_TRIGGER_NORM, trigger)                                                   // Mapping of a trigger position, normalized
#define INPUT_MAP_GAMEPAD_JOYSTICK(joystick, death_zone) _INPUT_MAP(INPUT_METHOD_GAMEPAD_JOYSTICK, INPUT_METHOD_GAMEPAD_JOYSTICK_INPUT_ID(joystick, death_zone))  // Mapping of a joystick position with death zone
// clang-format on

// Representation of an input mapping result
typedef struct {
    bool bool_v;  // `false` if the input is in its resting position, `true` otherwise
    f32 float_v;  // A continuous value of non-button inputs or `0-1` for buttons
} InputResult;

// Representation of all the input results for an input device
typedef struct {
    InputDeviceID device;  // Input device of the input results
    InputResult* results;  // Array of input results for this device
} InputDeviceResults;

// ----------------------------------------------------------------------------
// ---- Basic Input Handler ---------------------------------------------------
// ----------------------------------------------------------------------------

// Basic input handler - No pooling
typedef struct {
    InputMap* mappings;             // Array of input locations being the index the action ID of that location
    input_size size;                // Size of the actions array
    InputDeviceID device;           // Input device to check
    InputDeviceState device_state;  // The state of the device
} BasicInputHandler;

/**
 * Creates a basic input handler.
 * @param device Device associated to this input handler.
 * @param n_actions Number of actions that will be mapped.
 * @return A new basic input handler.
 */
BasicInputHandler BasicInputHandlerCreate(InputDeviceID device, input_size n_actions);
/**
 * Deletes a previously created basic input handler.
 * @param handler Basic input handler to delete.
 */
void BasicInputHandlerDelete(BasicInputHandler* handler);

/**
 * Maps an action ID with a input location.
 * @param handler Basic input handler to modify.
 * @param action_id Action ID to map.
 * @param map Input location to use for the action.
 * @return If the input mapping is valid for this handler.
 */
bool BasicInputHandlerMapSet(BasicInputHandler* handler, InputActionID action_id, const InputMap map);
/**
 * Maps all the action IDs with its corresponding input location. The mapping index represents the correponding action ID.
 * @param handler Basic input handler to modify.
 * @param mappings Array of input mappings to use for the actions with `size = handler.size`.
 * @return Number of invalid input mappings.
 */
u16 BasicInputHandlerMappingsSet(BasicInputHandler* handler, const InputMap mappings[]);

/**
 * Returns the value of the input mapping for an action ID.
 * @param handler Basic input handler to use.
 * @param action_id Action ID to check. Must be a valid action ID for this handler.
 * @return Input result of the input.
 */
InputResult BasicInputHandlerGetValue(BasicInputHandler handler, InputActionID action_id);

// ----------------------------------------------------------------------------
// ---- Greedy Input Handler --------------------------------------------------
// ----------------------------------------------------------------------------

// Greedy input handler - No pooling - Input handler to group keyboard & mouse and all gamepads
typedef struct {
    InputMap* keyboard_mouse_mappings;     // Array of keyboard and mouse input mappings being the index the action ID of that mapping
    InputMap* gamepad_mappings;            // Array of gamepad input mappings being the index the action ID of that mapping
    InputResult* results;                  // Input results of the last input check with the `active_device`
    input_size size;                       // Size of the actions and results array
    InputDeviceID active_device;           // Device used to fill the results array
    InputDeviceState active_device_state;  // The state of the active device
} GreedyInputHandler;

/**
 * Creates a greedy input handler.
 * @param device Device associated to this input handler.
 * @param n_actions Number of actions that will be mapped.
 * @return A new greedy input handler.
 */
GreedyInputHandler GreedyInputHandlerCreate(input_size n_actions);
/**
 * Deletes a previously created greedy input handler.
 * @param handler Greedy input handler to delete.
 */
void GreedyInputHandlerDelete(GreedyInputHandler* handler);

/**
 * Maps an action ID with a input location.
 * @param handler Greedy input handler to modify.
 * @param device_map Defines the target device of the map. For a gamepad device, any gamepad device ID is valid.
 * @param action_id Action ID to map.
 * @param map Input location to use for the action.
 * @return If the input mapping is valid for this device and handler.
 */
bool GreedyInputHandlerMapSet(GreedyInputHandler* handler, InputDeviceID device_map, InputActionID action_id, const InputMap map);
/**
 * Maps all the action IDs with its corresponding input location. The mapping index represents the correponding action ID.
 * @param handler Greedy input handler to modify.
 * @param device_map Defines the target device of the array of mappings. For a gamepad device, any gamepad device ID is valid.
 * @param mappings Array of input mappings to use for the actions with `size = handler.size`.
 * @return Number of invalid input mappings.
 */
u16 GreedyInputHandlerDeviceMappingsSet(GreedyInputHandler* handler, InputDeviceID device_map, const InputMap mappings[]);
/**
 * Maps all the action IDs with its corresponding input location. The mapping index represents the correponding action ID.
 * @param handler Greedy input handler to modify.
 * @param keyboard_mappings Array of keyboard and mouse input mappings to use for the action with `size = handler.size`.
 * @param gamepad_mappings Array of gamepad input mappings to use for the action with `size = handler.size`.
 * @return Number of invalid input mappings.
 */
u16 GreedyInputHandlerMappingsSet(GreedyInputHandler* handler, const InputMap keyboard_mappings[], const InputMap gamepad_mappings[]);

/**
 * Updates the greedy input handler to look for the currently used input device and calculates the inputs for that device.
 * This results are cached until the next invocation.
 * Should be called when wanting to check for new inputs (e.g. every game loop iteration).
 * @param handler Greedy input handler to use.
 * @return Reference to the input results for the current device.
 */
InputDeviceResults GreedyInputHandlerUpdate(GreedyInputHandler* handler);

/**
 * Returns the value of the input mapping for an action ID.
 * This does not recalculate an input, but retrieves the cached result.
 * @param handler Greedy input handler to use.
 * @param action_id Action ID to check. Must be a valid action ID for this handler.
 * @return Cached input result of the action.
 */
InputResult GreedyInputHandlerGetValue(GreedyInputHandler handler, InputActionID action_id);

/**
 * Returns the value of the input mapping for an action ID.
 * This does not recalculate an input, but retrieves the cached result.
 * @param handler Greedy input handler to use.
 * @return All the cached input results.
 */
InputResult* GreedyInputHandlerGetAllValues(GreedyInputHandler handler);

#endif  // INPUTS_H