#pragma once
#ifndef __SOURCE_CONTROLS_H_
#define __SOURCE_CONTROLS_H_

#include "raylib.h"
#include "types.h"

typedef enum InputType {
    MOVE_UP = 0, // 0
    MOVE_RIGHT, // 1
    MOVE_DOWN, // 2
    MOVE_LEFT, // 3
    /* ... */
    ABILITY_SHOOT, // 4
    ABILITY_MISSILE, // 5
    ABILITY_DASH, // 6
} InputType;

#define CONTROL_ENTRIES_COUNT 7

// ---- PC Inputs -------------------------------------------------------------

typedef enum InputMethodPC {
    INPUT_METHOD_PC_MOUSE = 0, // Mouse buttons
    INPUT_METHOD_PC_KEYBOARD, // Keyboard keys
} InputMethodPC;

typedef struct InputEntryPC {
    InputMethodPC method;
    u32 input;
} InputEntry;

typedef struct ControlsPC {
    InputEntryPC entries[CONTROL_ENTRIES_COUNT];

} ControlsPC;

InputEntryPC DEFAULT_CONTROLS_PC[CONTROL_ENTRIES_COUNT] = {
    { INPUT_METHOD_PC_KEYBOARD, KEY_UP }, { INPUT_METHOD_PC_KEYBOARD, KEY_RIGHT }, { INPUT_METHOD_PC_KEYBOARD, KEY_DOWN },
    { INPUT_METHOD_PC_KEYBOARD, KEY_LEFT }, { INPUT_METHOD_PC_MOUSE, MOUSE_BUTTON_LEFT }, { INPUT_METHOD_PC_MOUSE, MOUSE_BUTTON_RIGHT },
    { INPUT_METHOD_PC_KEYBOARD, KEY_SPACE }
};

// ---- Gamepad Inputs --------------------------------------------------------

typedef enum InputMethodGamepad {
    INPUT_METHOD_GAMEPAD_BUTTON = 0, // Gamepad buttons
    INPUT_METHOD_GAMEPAD_AXIS, // Gamepad axis
} InputMethodGamepad;

typedef struct InputEntryGamepad {
    InputMethodGamepad method;
    u32 input;
} InputEntryGamepad;

typedef struct ControlsGamepad {
    InputEntryGamepad entries[CONTROL_ENTRIES_COUNT];

} ControlsGamepad;

#define A __Generic(x)

InputEntryPC DEFAULT_CONTROLS_GAMEPAD[CONTROL_ENTRIES_COUNT] = { {
                                                                     INPUT_METHOD_GAMEPAD_BUTTON,
                                                                 },
    {
        INPUT_METHOD_GAMEPAD_BUTTON,
    },
    {
        INPUT_METHOD_GAMEPAD_BUTTON,
    },
    {
        INPUT_METHOD_GAMEPAD_BUTTON,
    },
    {
        INPUT_METHOD_GAMEPAD_BUTTON,
    },
    {
        INPUT_METHOD_GAMEPAD_BUTTON,
    },
    {
        INPUT_METHOD_GAMEPAD_BUTTON,
    } };

#endif // __SOURCE_CONTROLS_H_