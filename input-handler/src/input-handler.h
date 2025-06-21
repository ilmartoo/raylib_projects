#pragma once
#ifndef __INPUTS_H__
#define __INPUTS_H__

#include "rayconfig.h"  // Raylib configurations
#include "raylib.h"     // Raylib library
#include "types.h"      // Ilmarto's types
#include "float16.h"    //

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
    INPUT_DEVICE_STATE_INITIAL = 0,               // The input handler had just been initialized so the default device is selected
    INPUT_DEVICE_STATE_ACTIVE,                    // The active device is being used
    INPUT_DEVICE_STATE_IDLE,                      // The active device is idle and no other device is being used
    INPUT_DEVICE_STATE_MISSING,                   // The active device has gone missing
    INPUT_DEVICE_STATE_CHANGE_IDLE_2_ACTIVE,      // The active device was changed because it was idle and other was being used
    INPUT_DEVICE_STATE_CHANGE_MISSING_2_ACTIVE,   // The active device was changed because it went missing and other device was being used
    INPUT_DEVICE_STATE_CHANGE_MISSING_2_DEFAULT,  // The active device was changed because it went missing, so the default device was
                                                  // selected

} InputDeviceState;

// Method of input to associate an input id to the correct key, mouse position, button, trigger or joystick:
// - Keyboard & Mouse input method - Input method value has 1 in its least significant bit
// - Gamepad input method          - Input method value has 0 in its in least significant bit
typedef enum {
    // No input method
    METHOD_NONE = 0,

    // KEYBOARD KEYS //

    // Activates when a specified keyboard key is pressed (from UP position to DOWN position)
    // - Values `0, 1`
    METHOD_KEYBOARD_KEY_PRESSED = 1,
    // Activates when a specified keyboard key is released (from DOWN position to UP position)
    // - Values `0, 1`
    METHOD_KEYBOARD_KEY_RELEASED = 3,
    // Activates when a specified keyboard key is down
    // - Values `0, 1`
    METHOD_KEYBOARD_KEY_DOWN = 5,
    // Activates when a specified keyboard key is up
    // - Values `0, 1`
    METHOD_KEYBOARD_KEY_UP = 7,
    // Activates when any keyboard key is pressed (from UP position to DOWN position)
    // - Values `0, 1`
    // METHOD_KEYBOARD_ANY_KEY_PRESSED = 9,
    // Activates when any keyboard key is released (from DOWN position to UP position)
    // - Values `0, 1`
    // METHOD_KEYBOARD_KEY_RELEASED = 11,
    // Activates when any keyboard key is down
    // - Values `0, 1`
    // METHOD_KEYBOARD_ANY_KEY_DOWN = 13,
    // Activates when any keyboard key is up
    // - Values `0, 1`
    // METHOD_KEYBOARD_ANY_KEY_UP = 15,

    // MOUSE BUTTONS //

    // Activates when a specified mouse button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    METHOD_MOUSE_BUTTON_PRESSED = 17,
    // Activates when a specified mouse button is pressed (from DOWN position to UP position)
    // - Values: `0, 1`
    METHOD_MOUSE_BUTTON_RELEASED = 19,
    // Activates when a specified mouse button is down
    // - Values: `0, 1`
    METHOD_MOUSE_BUTTON_DOWN = 21,
    // Activates when a specified mouse button is up
    // - Values: `0, 1`
    METHOD_MOUSE_BUTTON_UP = 23,
    // Activates when any mouse button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    // METHOD_MOUSE_ANY_BUTTON_PRESSED = 25,
    // Activates when any mouse button is pressed (from DOWN position to UP position)
    // - Values: `0, 1`
    // METHOD_MOUSE_ANY_BUTTON_RELEASED = 27,
    // Activates when any mouse button is down
    // - Values: `0, 1`
    // METHOD_MOUSE_ANY_BUTTON_DOWN = 29,
    // Activates when any mouse button is up
    // - Values: `0, 1`
    // METHOD_MOUSE_ANY_BUTTON_UP = 31,

    // MOUSE MOVEMENTS //

    // Activates when the mouse moves with its current position
    // - Values: `FLOAT_MIN..FLOAT_MAX`
    // - Movement threshold can be encoded
    METHOD_MOUSE_POSITION = 33,
    // Activates when the mouse moves with the frame delta movement
    // - Values: `FLOAT_MIN..FLOAT_MAX`
    // - Movement threshold can be encoded
    METHOD_MOUSE_MOVEMENT = 35,
    // Activates when the mouse scrolls with the scrolled distance
    // - Values: `FLOAT_MIN..FLOAT_MAX`
    // - Scroll threshold can be encoded
    METHOD_MOUSE_SCROLL = 37,

    // GAMEPAD BUTTONS //

    // Activates when a specified gamepad button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    METHOD_GAMEPAD_BUTTON_PRESSED = 2,
    // Activates when a specified gamepad button is released (from DOWN position to UP position)
    // - Values `0, 1`
    METHOD_GAMEPAD_BUTTON_RELEASED = 4,
    // Activates when a specified gamepad button is down
    // - Values `0, 1`
    METHOD_GAMEPAD_BUTTON_DOWN = 6,
    // Activates when a specified gamepad button is up
    // - Values `0, 1`
    METHOD_GAMEPAD_BUTTON_UP = 8,
    // Activates when any gamepad button is pressed (from UP position to DOWN position)
    // - Values: `0, 1`
    // METHOD_GAMEPAD_ANY_BUTTON_PRESSED = 10,
    // Activates when any gamepad button is pressed (from DOWN position to UP position)
    // - Values: `0, 1`
    // METHOD_GAMEPAD_ANY_BUTTON_RELEASED = 12,
    // Activates when any gamepad button is down
    // - Values: `0, 1`
    // METHOD_GAMEPAD_ANY_BUTTON_DOWN = 14,
    // Activates when any gamepad button is up
    // - Values: `0, 1`
    // METHOD_GAMEPAD_ANY_BUTTON_UP = 16,

    // GAMEPAD TRIGGERS //

    // Activates when a specified gamepad trigger is pressed
    // - Values: `-1..1`
    // - Death zone threshold can be encoded
    METHOD_GAMEPAD_TRIGGER = 18,
    // Activates when a specified gamepad trigger is pressed (value normalized)
    // - Values: `0..1`
    // - Death zone threshold can be encoded (applied before normalization)
    METHOD_GAMEPAD_TRIGGER_NORM = 20,

    // GAMEPAD JOYSTICKS //

    // Activates when a specified gamepad joystick is moved along the specified axis
    // - Values: `-1..1`
    // - Death zone threshold can be encoded
    METHOD_GAMEPAD_JOYSTICK = 22,

} InputMethod;

#define IS_METHOD_FROM_KEYBOARD_AND_MOUSE(method) (method & 0b1)  // Check if is a valid keyboard & mouse input method

// Gamepad Trigger input
typedef enum {
    GAMEPAD_TRIGGER_LEFT = GAMEPAD_AXIS_LEFT_TRIGGER,
    GAMEPAD_TRIGGER_RIGHT = GAMEPAD_AXIS_RIGHT_TRIGGER,
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

typedef u16 action_size;

// An input action ID - The action IDs valid range from `0` to `2^16 - 1`
typedef action_size InputActionID;

// The valid precisions for the thresholds
typedef enum {
    ZERO_DECIMALS = 0,
    ONE_DECIMAL = 1,
    TWO_DECIMALS = 2,
    THREE_DECIMALS = 3,
    FOUR_DECIMALS = 4,
} ThresholdPrecission;

// An input mapping
typedef struct {
    u16 method;
    union {
        // METHOD_KEYBOARD_KEY_PRESSED
        // METHOD_KEYBOARD_KEY_RELEASED
        // METHOD_KEYBOARD_KEY_DOWN
        // METHOD_KEYBOARD_KEY_UP
        i32 key;  // KeyboardKey | other
        // METHOD_MOUSE_BUTTON_PRESSED
        // METHOD_MOUSE_BUTTON_RELEASED
        // METHOD_MOUSE_BUTTON_DOWN
        // METHOD_MOUSE_BUTTON_UP
        // METHOD_GAMEPAD_BUTTON_PRESSED
        // METHOD_GAMEPAD_BUTTON_RELEASED
        // METHOD_GAMEPAD_BUTTON_DOWN
        // METHOD_GAMEPAD_BUTTON_UP
        i32 button;  // MouseButton | GamepadButton | other
        // METHOD_MOUSE_POSITION
        // METHOD_MOUSE_MOVEMENT
        struct {
            u16 axis;  // MouseAxis
            u16 threshold;
        } movement;
        // METHOD_MOUSE_SCROLL
        struct {
            u16 type;  // MouseScroll
            u16 threshold;
        } scroll;
        // METHOD_GAMEPAD_TRIGGER
        // METHOD_GAMEPAD_TRIGGER_NORM
        // METHOD_GAMEPAD_JOYSTICK
        struct {
            u16 type;       // GamepadTrigger | GamepadJoystick | other
            f16 threshold;  // Threshold in half-precision floating point
        } axis;
    } data;
} InputMap;

#define _INPUT_MAP(method, id) ((InputMap){method, id})  // Input map creator helper

// No input //

#define MAP_NONE ((InputMap)(METHOD_NONE, 0))

// Keyboard keys input //

#define MAP_KEYBOARD_KEY_PRESSED(key)  ((InputMap){METHOD_KEYBOARD_KEY_PRESSED, {key}})
#define MAP_KEYBOARD_KEY_RELEASED(key) ((InputMap){METHOD_KEYBOARD_KEY_RELEASED, {key}})
#define MAP_KEYBOARD_KEY_DOWN(key)     ((InputMap){METHOD_KEYBOARD_KEY_DOWN, {key}})
#define MAP_KEYBOARD_KEY_UP(key)       ((InputMap){METHOD_KEYBOARD_KEY_UP, {key}})

// Mouse buttons input //

#define MAP_MOUSE_BUTTON_PRESSED(button)  ((InputMap){METHOD_MOUSE_BUTTON_PRESSED, {button}})
#define MAP_MOUSE_BUTTON_RELEASED(button) ((InputMap){METHOD_MOUSE_BUTTON_RELEASED, {button}})
#define MAP_MOUSE_BUTTON_DOWN(button)     ((InputMap){METHOD_MOUSE_BUTTON_DOWN, {button}})
#define MAP_MOUSE_BUTTON_UP(button)       ((InputMap){METHOD_MOUSE_BUTTON_UP, {button}})

// Mouse movements input //

#define MAP_MOUSE_POSITION(axis)              ((InputMap){METHOD_MOUSE_POSITION, {.movement = {axis, 0}}})
#define MAP_MOUSE_MOVEMENT(axis)              ((InputMap){METHOD_MOUSE_MOVEMENT, {.movement = {axis, 0}}})
#define MAP_MOUSE_SCROLL(type)                ((InputMap){METHOD_MOUSE_SCROLL, {.scroll = {type, 0}}})
#define MAP_MOUSE_POSITION_T(axis, threshold) ((InputMap){METHOD_MOUSE_POSITION, {.movement = {axis, threshold}}})
#define MAP_MOUSE_MOVEMENT_T(axis, threshold) ((InputMap){METHOD_MOUSE_MOVEMENT, {.movement = {axis, threshold}}})
#define MAP_MOUSE_SCROLL_T(type, threshold)   ((InputMap){METHOD_MOUSE_SCROLL, {.scroll = {type, threshold}}})

// Gamepad buttons input //

#define MAP_GAMEPAD_BUTTON_PRESSED(button)  ((InputMap){METHOD_GAMEPAD_BUTTON_PRESSED, {button}})
#define MAP_GAMEPAD_BUTTON_RELEASED(button) ((InputMap){METHOD_GAMEPAD_BUTTON_RELEASED, {button}})
#define MAP_GAMEPAD_BUTTON_DOWN(button)     ((InputMap){METHOD_GAMEPAD_BUTTON_DOWN, {button}})
#define MAP_GAMEPAD_BUTTON_UP(button)       ((InputMap){METHOD_GAMEPAD_BUTTON_UP, {button}})

// Gamepad triggers input //

#define MAP_GAMEPAD_TRIGGER(trigger)                   ((InputMap){METHOD_GAMEPAD_TRIGGER, {.axis = {trigger, 0}}})
#define MAP_GAMEPAD_TRIGGER_NORM(trigger)              ((InputMap){METHOD_GAMEPAD_TRIGGER_NORM, {.axis = {trigger, 0}}})
#define MAP_GAMEPAD_TRIGGER_T(trigger, threshold)      ((InputMap){METHOD_GAMEPAD_TRIGGER, {.axis = {trigger, ftof16(threshold)}}})
#define MAP_GAMEPAD_TRIGGER_NORM_T(trigger, threshold) ((InputMap){METHOD_GAMEPAD_TRIGGER_NORM, {.axis = {trigger, ftof16(threshold)}}})

// Gamepad joysticks input //

#define MAP_GAMEPAD_JOYSTICK(joystick)              ((InputMap){METHOD_GAMEPAD_JOYSTICK, {.axis = {joystick, 0}}})
#define MAP_GAMEPAD_JOYSTICK_T(joystick, threshold) ((InputMap){METHOD_GAMEPAD_JOYSTICK, {.axis = {joystick, ftof16(threshold)}}})

// Representation of an input mapping result
typedef struct {
    bool bool_v;  // `false` if the input is in its resting position, `true` otherwise
    f32 float_v;  // A continuous value of non-button inputs or `0-1` for buttons
} InputResult;

// Representation of all the input results for an input device
typedef struct {
    InputDeviceID device;  // Input device of the input results
    InputResult *results;  // Array of input results for this device
} InputDeviceResults;

// ----------------------------------------------------------------------------
// ---- Basic Input Handler ---------------------------------------------------
// ----------------------------------------------------------------------------

// Basic input handler - No pooling
typedef struct {
    InputMap *mappings;    // Array of input locations being the index the action ID of that location
    action_size size;      // Size of the actions array
    InputDeviceID device;  // Input device to check
    u8 device_state;       // The state of the device
} BasicInputHandler;

/**
 * Creates a basic input handler.
 * @param device Device associated to this input handler.
 * @param n_actions Number of actions that will be mapped.
 * @return A new basic input handler.
 */
BasicInputHandler BasicInputHandlerCreate(InputDeviceID device, action_size n_actions);
/**
 * Deletes a previously created basic input handler.
 * @param handler Basic input handler to delete.
 */
void BasicInputHandlerDelete(BasicInputHandler *handler);

/**
 * Maps an action ID with a input location.
 * @param handler Basic input handler to modify.
 * @param action_id Action ID to map.
 * @param map Input location to use for the action.
 * @return If the input mapping is valid for this handler.
 */
bool BasicInputHandlerMapSet(BasicInputHandler *handler, InputActionID action_id, const InputMap map);
/**
 * Maps all the action IDs with its corresponding input location. The mapping index represents the correponding action ID.
 * @param handler Basic input handler to modify.
 * @param mappings Array of input mappings to use for the actions with `size = handler.size`.
 * @return Number of invalid input mappings.
 */
action_size BasicInputHandlerMappingsSet(BasicInputHandler *handler, const InputMap mappings[]);

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
    InputMap *keyboard_mouse_mappings;  // Array of keyboard and mouse input mappings being the index the action ID of that mapping
    InputMap *gamepad_mappings;         // Array of gamepad input mappings being the index the action ID of that mapping
    InputResult *results;               // Input results of the last input check with the `active_device`
    action_size size;                   // Size of the actions and results array
    InputDeviceID active_device;        // Device used to fill the results array
    u8 active_device_state;             // The state of the active device
} GreedyInputHandler;

/**
 * Creates a greedy input handler.
 * @param device Device associated to this input handler.
 * @param n_actions Number of actions that will be mapped.
 * @return A new greedy input handler.
 */
GreedyInputHandler GreedyInputHandlerCreate(action_size n_actions);
/**
 * Deletes a previously created greedy input handler.
 * @param handler Greedy input handler to delete.
 */
void GreedyInputHandlerDelete(GreedyInputHandler *handler);

/**
 * Maps an action ID with a input location.
 * @param handler Greedy input handler to modify.
 * @param device_map Defines the target device of the map. For a gamepad device, any gamepad device ID is valid.
 * @param action_id Action ID to map.
 * @param map Input location to use for the action.
 * @return If the input mapping is valid for this device and handler.
 */
bool GreedyInputHandlerMapSet(GreedyInputHandler *handler, InputDeviceID device_map, InputActionID action_id, const InputMap map);
/**
 * Maps all the action IDs with its corresponding input location. The mapping index represents the correponding action ID.
 * @param handler Greedy input handler to modify.
 * @param device_map Defines the target device of the array of mappings. For a gamepad device, any gamepad device ID is valid.
 * @param mappings Array of input mappings to use for the actions with `size = handler.size`.
 * @return Number of invalid input mappings.
 */
action_size GreedyInputHandlerDeviceMappingsSet(GreedyInputHandler *handler, InputDeviceID device_map, const InputMap mappings[]);
/**
 * Maps all the action IDs with its corresponding input location. The mapping index represents the correponding action ID.
 * @param handler Greedy input handler to modify.
 * @param keyboard_mappings Array of keyboard and mouse input mappings to use for the action with `size = handler.size`.
 * @param gamepad_mappings Array of gamepad input mappings to use for the action with `size = handler.size`.
 * @return Number of invalid input mappings.
 */
action_size GreedyInputHandlerMappingsSet(GreedyInputHandler *handler,
                                          const InputMap keyboard_mappings[],
                                          const InputMap gamepad_mappings[]);

/**
 * Updates the greedy input handler to look for the currently used input device and calculates the inputs for that device.
 * This results are cached until the next invocation.
 * Should be called when wanting to check for new inputs (e.g. every game loop iteration).
 * @param handler Greedy input handler to use.
 * @return Reference to the input results for the current device.
 */
InputDeviceResults GreedyInputHandlerUpdate(GreedyInputHandler *handler);

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
InputResult *GreedyInputHandlerGetAllValues(GreedyInputHandler handler);

#endif  // __INPUTS_H__