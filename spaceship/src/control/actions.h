#pragma once
#ifndef __ACTIONS_H__
#define __ACTIONS_H__

#include "input/input-handler.h"

// ----------------------------------------------------------------------------
// ---- Actions ---------------------------------------------------------------
// ----------------------------------------------------------------------------

typedef enum Action {
    ACTION_PAUSE = 0,

    ACTION_MOVE_UP,
    ACTION_MOVE_RIGHT,
    ACTION_MOVE_DOWN,
    ACTION_MOVE_LEFT,

    ACTION_AIM_X,
    ACTION_AIM_Y,

    ACTION_ABILITY_SHOOT,
    ACTION_ABILITY_MISSILE,
    ACTION_ABILITY_DASH,
} Action;

#define ACTION_TYPES_COUNT 10

// ----------------------------------------------------------------------------
// ---- Defaults --------------------------------------------------------------
// ----------------------------------------------------------------------------

InputMap default_kbm_maps[ACTION_TYPES_COUNT] = {
    MAP_KEYBOARD_KEY_PRESSED(KEY_P), /* ACTION_PAUSE */

    MAP_KEYBOARD_KEY_DOWN(KEY_W), /* ACTION_MOVE_UP */
    MAP_KEYBOARD_KEY_DOWN(KEY_D), /* ACTION_MOVE_RIGHT */
    MAP_KEYBOARD_KEY_DOWN(KEY_S), /* ACTION_MOVE_DOWN */
    MAP_KEYBOARD_KEY_DOWN(KEY_A), /* ACTION_MOVE_LEFT */

    MAP_MOUSE_POSITION(MOUSE_AXIS_X), /* ACTION_AIM_X */
    MAP_MOUSE_POSITION(MOUSE_AXIS_Y), /* ACTION_AIM_Y */

    MAP_MOUSE_BUTTON_DOWN(MOUSE_BUTTON_LEFT),  /* ACTION_ABILITY_SHOOT */
    MAP_MOUSE_BUTTON_DOWN(MOUSE_BUTTON_RIGHT), /* ACTION_ABILITY_MISSILE */
    MAP_KEYBOARD_KEY_DOWN(KEY_SPACE),          /* ACTION_ABILITY_DASH */
};

InputMap default_gpd_maps[ACTION_TYPES_COUNT] = {
    MAP_GAMEPAD_BUTTON_PRESSED(GAMEPAD_BUTTON_MIDDLE_RIGHT), /* ACTION_PAUSE */

    MAP_KEYBOARD_KEY_DOWN(KEY_W), /* ACTION_MOVE_UP */
    MAP_KEYBOARD_KEY_DOWN(KEY_D), /* ACTION_MOVE_RIGHT */
    MAP_KEYBOARD_KEY_DOWN(KEY_S), /* ACTION_MOVE_DOWN */
    MAP_KEYBOARD_KEY_DOWN(KEY_A), /* ACTION_MOVE_LEFT */

    MAP_MOUSE_POSITION(MOUSE_AXIS_X), /* ACTION_AIM_X */
    MAP_MOUSE_POSITION(MOUSE_AXIS_Y), /* ACTION_AIM_Y */

    MAP_MOUSE_BUTTON_DOWN(MOUSE_BUTTON_LEFT),  /* ACTION_ABILITY_SHOOT */
    MAP_MOUSE_BUTTON_DOWN(MOUSE_BUTTON_RIGHT), /* ACTION_ABILITY_MISSILE */
    MAP_KEYBOARD_KEY_DOWN(KEY_SPACE),          /* ACTION_ABILITY_DASH */
};

#define ACTION_MAPPING_DEFAULT_GAMEPAD                                                             \
    {                                                                                              \
        {INPUT_METHOD_GAMEPAD_BUTTON, GAMEPAD_BUTTON_MIDDLE_RIGHT}, /* ACTION_PAUSE */             \
                                                                                                   \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_NEGATIVE_Y}, /* ACTION_MOVE_UP */    \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_POSITIVE_X}, /* ACTION_MOVE_RIGHT */ \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_POSITIVE_Y}, /* ACTION_MOVE_DOWN */  \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_LEFT_NEGATIVE_X}, /* ACTION_MOVE_LEFT */  \
                                                                                                   \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_NEGATIVE_Y}, /* ACTION_AIM_UP */    \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_POSITIVE_X}, /* ACTION_AIM_RIGHT */ \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_POSITIVE_Y}, /* ACTION_AIM_DOWN */  \
        {INPUT_METHOD_GAMEPAD_JOYSTICK, GAMEPAD_JOYSTICK_RIGHT_NEGATIVE_X}, /* ACTION_AIM_LEFT */  \
                                                                                                   \
        {INPUT_METHOD_GAMEPAD_TRIGGER, GAMEPAD_TRIGGER_RIGHT},        /* ACTION_ABILITY_SHOOT */   \
        {INPUT_METHOD_GAMEPAD_TRIGGER, GAMEPAD_TRIGGER_LEFT},         /* ACTION_ABILITY_MISSILE */ \
        {INPUT_METHOD_GAMEPAD_BUTTON, GAMEPAD_BUTTON_LEFT_TRIGGER_1}, /* ACTION_ABILITY_DASH */    \
    }

#endif  // __ACTIONS_H__