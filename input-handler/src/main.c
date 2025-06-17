#include "input-handler.h"
#include "rayconfig.h"
#include "raylib.h"
#include "types.h"

#define SCREEN_WIDTH  1400  // Screen width
#define SCREEN_HEIGHT 1000  // Screen height

#define N_ACTIONS 122  // Size of the actions array

#define JOYSTICK_DEATH_ZONE 800  // Joystick death zone

#define FPS_PADDING_X 140  // FPS text horizontal padding from screen border
#define FPS_PADDING_Y 30   // FPS text vertical padding from screen border

#define FONT_SIZE    32        // Font size for rendering
#define FONT_COLOR   RAYWHITE  // Font color
#define TEXT_PADDING 30        // Text padding from screen border
#define TEXT_SPACING 6         // Text spacing between lines

// String type
typedef struct {
    char str[1 << 8];
} String;

String keyboard_key_text(KeyboardKey key) {
    switch (key) {
        case KEY_NULL: return (String){"Null Key"};
        case KEY_APOSTROPHE: return (String){"'"};
        case KEY_COMMA: return (String){","};
        case KEY_MINUS: return (String){"-"};
        case KEY_PERIOD: return (String){"."};
        case KEY_SLASH: return (String){"/"};
        case KEY_ZERO: return (String){"0"};
        case KEY_ONE: return (String){"1"};
        case KEY_TWO: return (String){"2"};
        case KEY_THREE: return (String){"3"};
        case KEY_FOUR: return (String){"4"};
        case KEY_FIVE: return (String){"5"};
        case KEY_SIX: return (String){"6"};
        case KEY_SEVEN: return (String){"7"};
        case KEY_EIGHT: return (String){"8"};
        case KEY_NINE: return (String){"9"};
        case KEY_SEMICOLON: return (String){";"};
        case KEY_EQUAL: return (String){"="};
        case KEY_A: return (String){"A"};
        case KEY_B: return (String){"B"};
        case KEY_C: return (String){"C"};
        case KEY_D: return (String){"D"};
        case KEY_E: return (String){"E"};
        case KEY_F: return (String){"F"};
        case KEY_G: return (String){"G"};
        case KEY_H: return (String){"H"};
        case KEY_I: return (String){"I"};
        case KEY_J: return (String){"J"};
        case KEY_K: return (String){"K"};
        case KEY_L: return (String){"L"};
        case KEY_M: return (String){"M"};
        case KEY_N: return (String){"N"};
        case KEY_O: return (String){"O"};
        case KEY_P: return (String){"P"};
        case KEY_Q: return (String){"Q"};
        case KEY_R: return (String){"R"};
        case KEY_S: return (String){"S"};
        case KEY_T: return (String){"T"};
        case KEY_U: return (String){"U"};
        case KEY_V: return (String){"V"};
        case KEY_W: return (String){"W"};
        case KEY_X: return (String){"X"};
        case KEY_Y: return (String){"Y"};
        case KEY_Z: return (String){"Z"};
        case KEY_LEFT_BRACKET: return (String){"["};
        case KEY_BACKSLASH: return (String){"\\"};
        case KEY_RIGHT_BRACKET: return (String){"]"};
        case KEY_GRAVE: return (String){"`"};
        case KEY_SPACE: return (String){"Space"};
        case KEY_ESCAPE: return (String){"Escape"};
        case KEY_ENTER: return (String){"Enter"};
        case KEY_TAB: return (String){"Tab"};
        case KEY_BACKSPACE: return (String){"Backspace"};
        case KEY_INSERT: return (String){"Insert"};
        case KEY_DELETE: return (String){"Delete"};
        case KEY_RIGHT: return (String){"Right"};
        case KEY_LEFT: return (String){"Left"};
        case KEY_DOWN: return (String){"Down"};
        case KEY_UP: return (String){"Up"};
        case KEY_PAGE_UP: return (String){"Page Up"};
        case KEY_PAGE_DOWN: return (String){"Page Down"};
        case KEY_HOME: return (String){"Home"};
        case KEY_END: return (String){"End"};
        case KEY_CAPS_LOCK: return (String){"Caps Lock"};
        case KEY_SCROLL_LOCK: return (String){"Scroll Lock"};
        case KEY_NUM_LOCK: return (String){"Num Lock"};
        case KEY_PRINT_SCREEN: return (String){"Print Screen"};
        case KEY_PAUSE: return (String){"Pause"};
        case KEY_F1: return (String){"F1"};
        case KEY_F2: return (String){"F2"};
        case KEY_F3: return (String){"F3"};
        case KEY_F4: return (String){"F4"};
        case KEY_F5: return (String){"F5"};
        case KEY_F6: return (String){"F6"};
        case KEY_F7: return (String){"F7"};
        case KEY_F8: return (String){"F8"};
        case KEY_F9: return (String){"F9"};
        case KEY_F10: return (String){"F10"};
        case KEY_F11: return (String){"F11"};
        case KEY_F12: return (String){"F12"};
        case KEY_LEFT_SHIFT: return (String){"Shift (Left)"};
        case KEY_LEFT_CONTROL: return (String){"Control (Left)"};
        case KEY_LEFT_ALT: return (String){"Alt (Left)"};
        case KEY_LEFT_SUPER: return (String){"Command (Left)"};
        case KEY_RIGHT_SHIFT: return (String){"Shift (Right)"};
        case KEY_RIGHT_CONTROL: return (String){"Control (Right)"};
        case KEY_RIGHT_ALT: return (String){"Alt (Right)"};
        case KEY_RIGHT_SUPER: return (String){"Command (Right)"};
        case KEY_KB_MENU: return (String){"Menu"};
        case KEY_KP_0: return (String){"0 (Keypad)"};
        case KEY_KP_1: return (String){"1 (Keypad)"};
        case KEY_KP_2: return (String){"2 (Keypad)"};
        case KEY_KP_3: return (String){"3 (Keypad)"};
        case KEY_KP_4: return (String){"4 (Keypad)"};
        case KEY_KP_5: return (String){"5 (Keypad)"};
        case KEY_KP_6: return (String){"6 (Keypad)"};
        case KEY_KP_7: return (String){"7 (Keypad)"};
        case KEY_KP_8: return (String){"8 (Keypad)"};
        case KEY_KP_9: return (String){"9 (Keypad)"};
        case KEY_KP_DECIMAL: return (String){". (Keypad)"};
        case KEY_KP_DIVIDE: return (String){"\\ (Keypad)"};
        case KEY_KP_MULTIPLY: return (String){"* (Keypad)"};
        case KEY_KP_SUBTRACT: return (String){"- (Keypad)"};
        case KEY_KP_ADD: return (String){"+ (Keypad)"};
        case KEY_KP_ENTER: return (String){"Enter (Keypad)"};
        case KEY_KP_EQUAL: return (String){"= (Keypad)"};
        case KEY_BACK: return (String){"Back (Android)"};                // Android
        case KEY_MENU: return (String){"Menu (Android)"};                // Android
        case KEY_VOLUME_UP: return (String){"Volume Up (Android)"};      // Android
        case KEY_VOLUME_DOWN: return (String){"Volume Down (Android)"};  // Android
        default: return (String){"Unkown Keyboard Key"};
    }
}

String mouse_button_text(MouseButton button) {
    switch (button) {
        case MOUSE_BUTTON_LEFT: return (String){"Mouse Left"};
        case MOUSE_BUTTON_RIGHT: return (String){"Mouse Right"};
        case MOUSE_BUTTON_MIDDLE: return (String){"Mouse Middle"};
        case MOUSE_BUTTON_SIDE: return (String){"Mouse Side"};
        case MOUSE_BUTTON_EXTRA: return (String){"Mouse Extra"};
        case MOUSE_BUTTON_FORWARD: return (String){"Mouse Forward"};
        case MOUSE_BUTTON_BACK: return (String){"Mouse Back"};
        default: return (String){"Unkown Mouse Button"};
    }
}

String mouse_position_text(MouseAxis axis) {
    switch (axis) {
        case MOUSE_AXIS_X: return (String){"Mouse Position X"};
        case MOUSE_AXIS_Y: return (String){"Mouse Position Y"};
        default: return (String){"Unkown Mouse Position Axis"};
    }
}

String mouse_movement_text(MouseAxis axis) {
    switch (axis) {
        case MOUSE_AXIS_X: return (String){"Mouse Movement X"};
        case MOUSE_AXIS_Y: return (String){"Mouse Movement Y"};
        default: return (String){"Unkown Mouse Movement Axis"};
    }
}

String mouse_scroll_text(MouseScroll scroll) {
    switch (scroll) {
        case MOUSE_SCROLL_WHEEL: return (String){"Mouse Scroll Wheel"};
        default: return (String){"Unkown Mouse Scroll"};
    }
}

String gamepad_button_text(GamepadButton button) {
    switch (button) {
        case GAMEPAD_BUTTON_UNKNOWN: return (String){"Null Button"};
        case GAMEPAD_BUTTON_LEFT_FACE_UP: return (String){"Up Button (Left Face)"};
        case GAMEPAD_BUTTON_LEFT_FACE_RIGHT: return (String){"Right Button (Left Face)"};
        case GAMEPAD_BUTTON_LEFT_FACE_DOWN: return (String){"Down Button (Left Face)"};
        case GAMEPAD_BUTTON_LEFT_FACE_LEFT: return (String){"Left Button (Left Face)"};
        case GAMEPAD_BUTTON_RIGHT_FACE_UP: return (String){"Up Button (Right Face)"};
        case GAMEPAD_BUTTON_RIGHT_FACE_RIGHT: return (String){"Right Button (Right Face)"};
        case GAMEPAD_BUTTON_RIGHT_FACE_DOWN: return (String){"Down Button (Right Face)"};
        case GAMEPAD_BUTTON_RIGHT_FACE_LEFT: return (String){"Left Button (Right Face)"};
        case GAMEPAD_BUTTON_LEFT_TRIGGER_1: return (String){"Left Up Trigger Button"};
        case GAMEPAD_BUTTON_LEFT_TRIGGER_2: return (String){"Left Down Trigger Button"};
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_1: return (String){"Right Up Trigger Button"};
        case GAMEPAD_BUTTON_RIGHT_TRIGGER_2: return (String){"Right Down Trigger Button"};
        case GAMEPAD_BUTTON_MIDDLE_LEFT: return (String){"Middle Left Button"};
        case GAMEPAD_BUTTON_MIDDLE: return (String){"Middle Button"};
        case GAMEPAD_BUTTON_MIDDLE_RIGHT: return (String){"Middle Right Button"};
        case GAMEPAD_BUTTON_LEFT_THUMB: return (String){"Left Joystick Button"};
        case GAMEPAD_BUTTON_RIGHT_THUMB: return (String){"Right Joystick Button"};
        default: return (String){"Unkown Gamepad Button"};
    }
}

String gamepad_trigger_text(GamepadTrigger trigger) {
    switch (trigger) {
        case GAMEPAD_TRIGGER_LEFT: return (String){"Left Trigger (Pressure)"};
        case GAMEPAD_TRIGGER_RIGHT: return (String){"Right Trigger (Pressure)"};
        default: return (String){"Unkown Gamepad Trigger (Pressure)"};
    }
}

String gamepad_trigger_normalized_text(GamepadTrigger trigger) {
    switch (trigger) {
        case GAMEPAD_TRIGGER_LEFT: return (String){"Left Trigger Normalized (Pressure)"};
        case GAMEPAD_TRIGGER_RIGHT: return (String){"Right Trigger Normalized (Pressure)"};
        default: return (String){"Unkown Gamepad Trigger Normalized (Pressure)"};
    }
}

String gamepad_joystick_text(GamepadJoystick joystick) {
    switch (INPUT_METHOD_GAMEPAD_JOYSTICK_GET_JOYSTICK(joystick)) {
        case GAMEPAD_JOYSTICK_LEFT_X: return (String){"Left Joystick (X Axis)"};
        case GAMEPAD_JOYSTICK_LEFT_Y: return (String){"Left Joystick (Y Axis)"};
        case GAMEPAD_JOYSTICK_RIGHT_X: return (String){"Right Joystick (X Axis)"};
        case GAMEPAD_JOYSTICK_RIGHT_Y: return (String){"Right Joystick (Y Axis)"};
        default: return (String){"Unkown Gamepad Joytick"};
    }
}

void draw_screen_text(Font font, const char *text, u32 x, u32 y) { DrawTextEx(font, text, (Vector2){x, y}, FONT_SIZE, 1, FONT_COLOR); }

i32 main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Input Handler - Demo");
    SetExitKey(KEY_NULL);
    SetTargetFPS(0);

    Font font = LoadFont("assets/fonts/SpaceMono-Regular.ttf");
    SetTextLineSpacing(16);

    InputMap keyboard_mouse_locations[N_ACTIONS] = {
        INPUT_MAP_KEYBOARD_KEY(KEY_NULL),
        // Keyboard
        INPUT_MAP_KEYBOARD_KEY(KEY_APOSTROPHE),
        INPUT_MAP_KEYBOARD_KEY(KEY_COMMA),
        INPUT_MAP_KEYBOARD_KEY(KEY_MINUS),
        INPUT_MAP_KEYBOARD_KEY(KEY_PERIOD),
        INPUT_MAP_KEYBOARD_KEY(KEY_SLASH),
        INPUT_MAP_KEYBOARD_KEY(KEY_ZERO),
        INPUT_MAP_KEYBOARD_KEY(KEY_ONE),
        INPUT_MAP_KEYBOARD_KEY(KEY_TWO),
        INPUT_MAP_KEYBOARD_KEY(KEY_THREE),
        INPUT_MAP_KEYBOARD_KEY(KEY_FOUR),
        INPUT_MAP_KEYBOARD_KEY(KEY_FIVE),
        INPUT_MAP_KEYBOARD_KEY(KEY_SIX),
        INPUT_MAP_KEYBOARD_KEY(KEY_SEVEN),
        INPUT_MAP_KEYBOARD_KEY(KEY_EIGHT),
        INPUT_MAP_KEYBOARD_KEY(KEY_NINE),
        INPUT_MAP_KEYBOARD_KEY(KEY_SEMICOLON),
        INPUT_MAP_KEYBOARD_KEY(KEY_EQUAL),
        INPUT_MAP_KEYBOARD_KEY(KEY_A),
        INPUT_MAP_KEYBOARD_KEY(KEY_B),
        INPUT_MAP_KEYBOARD_KEY(KEY_C),
        INPUT_MAP_KEYBOARD_KEY(KEY_D),
        INPUT_MAP_KEYBOARD_KEY(KEY_E),
        INPUT_MAP_KEYBOARD_KEY(KEY_F),
        INPUT_MAP_KEYBOARD_KEY(KEY_G),
        INPUT_MAP_KEYBOARD_KEY(KEY_H),
        INPUT_MAP_KEYBOARD_KEY(KEY_I),
        INPUT_MAP_KEYBOARD_KEY(KEY_J),
        INPUT_MAP_KEYBOARD_KEY(KEY_K),
        INPUT_MAP_KEYBOARD_KEY(KEY_L),
        INPUT_MAP_KEYBOARD_KEY(KEY_M),
        INPUT_MAP_KEYBOARD_KEY(KEY_N),
        INPUT_MAP_KEYBOARD_KEY(KEY_O),
        INPUT_MAP_KEYBOARD_KEY(KEY_P),
        INPUT_MAP_KEYBOARD_KEY(KEY_Q),
        INPUT_MAP_KEYBOARD_KEY(KEY_R),
        INPUT_MAP_KEYBOARD_KEY(KEY_S),
        INPUT_MAP_KEYBOARD_KEY(KEY_T),
        INPUT_MAP_KEYBOARD_KEY(KEY_U),
        INPUT_MAP_KEYBOARD_KEY(KEY_V),
        INPUT_MAP_KEYBOARD_KEY(KEY_W),
        INPUT_MAP_KEYBOARD_KEY(KEY_X),
        INPUT_MAP_KEYBOARD_KEY(KEY_Y),
        INPUT_MAP_KEYBOARD_KEY(KEY_Z),
        INPUT_MAP_KEYBOARD_KEY(KEY_LEFT_BRACKET),
        INPUT_MAP_KEYBOARD_KEY(KEY_BACKSLASH),
        INPUT_MAP_KEYBOARD_KEY(KEY_RIGHT_BRACKET),
        INPUT_MAP_KEYBOARD_KEY(KEY_GRAVE),
        INPUT_MAP_KEYBOARD_KEY(KEY_SPACE),
        INPUT_MAP_KEYBOARD_KEY(KEY_ESCAPE),
        INPUT_MAP_KEYBOARD_KEY(KEY_ENTER),
        INPUT_MAP_KEYBOARD_KEY(KEY_TAB),
        INPUT_MAP_KEYBOARD_KEY(KEY_BACKSPACE),
        INPUT_MAP_KEYBOARD_KEY(KEY_INSERT),
        INPUT_MAP_KEYBOARD_KEY(KEY_DELETE),
        INPUT_MAP_KEYBOARD_KEY(KEY_RIGHT),
        INPUT_MAP_KEYBOARD_KEY(KEY_LEFT),
        INPUT_MAP_KEYBOARD_KEY(KEY_DOWN),
        INPUT_MAP_KEYBOARD_KEY(KEY_UP),
        INPUT_MAP_KEYBOARD_KEY(KEY_PAGE_UP),
        INPUT_MAP_KEYBOARD_KEY(KEY_PAGE_DOWN),
        INPUT_MAP_KEYBOARD_KEY(KEY_HOME),
        INPUT_MAP_KEYBOARD_KEY(KEY_END),
        INPUT_MAP_KEYBOARD_KEY(KEY_CAPS_LOCK),
        INPUT_MAP_KEYBOARD_KEY(KEY_SCROLL_LOCK),
        INPUT_MAP_KEYBOARD_KEY(KEY_NUM_LOCK),
        INPUT_MAP_KEYBOARD_KEY(KEY_PRINT_SCREEN),
        INPUT_MAP_KEYBOARD_KEY(KEY_PAUSE),
        INPUT_MAP_KEYBOARD_KEY(KEY_F1),
        INPUT_MAP_KEYBOARD_KEY(KEY_F2),
        INPUT_MAP_KEYBOARD_KEY(KEY_F3),
        INPUT_MAP_KEYBOARD_KEY(KEY_F4),
        INPUT_MAP_KEYBOARD_KEY(KEY_F5),
        INPUT_MAP_KEYBOARD_KEY(KEY_F6),
        INPUT_MAP_KEYBOARD_KEY(KEY_F7),
        INPUT_MAP_KEYBOARD_KEY(KEY_F8),
        INPUT_MAP_KEYBOARD_KEY(KEY_F9),
        INPUT_MAP_KEYBOARD_KEY(KEY_F10),
        INPUT_MAP_KEYBOARD_KEY(KEY_F11),
        INPUT_MAP_KEYBOARD_KEY(KEY_F12),
        INPUT_MAP_KEYBOARD_KEY(KEY_LEFT_SHIFT),
        INPUT_MAP_KEYBOARD_KEY(KEY_LEFT_CONTROL),
        INPUT_MAP_KEYBOARD_KEY(KEY_LEFT_ALT),
        INPUT_MAP_KEYBOARD_KEY(KEY_LEFT_SUPER),
        INPUT_MAP_KEYBOARD_KEY(KEY_RIGHT_SHIFT),
        INPUT_MAP_KEYBOARD_KEY(KEY_RIGHT_CONTROL),
        INPUT_MAP_KEYBOARD_KEY(KEY_RIGHT_ALT),
        INPUT_MAP_KEYBOARD_KEY(KEY_RIGHT_SUPER),
        INPUT_MAP_KEYBOARD_KEY(KEY_KB_MENU),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_0),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_1),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_2),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_3),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_4),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_5),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_6),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_7),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_8),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_9),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_DECIMAL),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_DIVIDE),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_MULTIPLY),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_SUBTRACT),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_ADD),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_ENTER),
        INPUT_MAP_KEYBOARD_KEY(KEY_KP_EQUAL),
        INPUT_MAP_KEYBOARD_KEY(KEY_BACK),
        INPUT_MAP_KEYBOARD_KEY(KEY_MENU),
        INPUT_MAP_KEYBOARD_KEY(KEY_VOLUME_UP),
        INPUT_MAP_KEYBOARD_KEY(KEY_VOLUME_DOWN),
        // Mouse button
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_LEFT),
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_RIGHT),
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_MIDDLE),
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_SIDE),
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_EXTRA),
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_FORWARD),
        INPUT_MAP_MOUSE_BUTTON(MOUSE_BUTTON_BACK),
        // Mouse position
        INPUT_MAP_MOUSE_POSITION(MOUSE_AXIS_X),
        INPUT_MAP_MOUSE_POSITION(MOUSE_AXIS_Y),
        // Mouse movement
        INPUT_MAP_MOUSE_MOVEMENT(MOUSE_AXIS_X),
        INPUT_MAP_MOUSE_MOVEMENT(MOUSE_AXIS_Y),
        // Mouse scroll
        INPUT_MAP_MOUSE_SCROLL(MOUSE_SCROLL_WHEEL),
    };
    InputMap gamepad_locations[N_ACTIONS] = {
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_UNKNOWN),
        // Buttons
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_FACE_UP),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_FACE_RIGHT),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_FACE_DOWN),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_FACE_LEFT),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_FACE_UP),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_FACE_RIGHT),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_FACE_DOWN),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_FACE_LEFT),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_TRIGGER_1),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_TRIGGER_2),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_TRIGGER_1),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_TRIGGER_2),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_MIDDLE_LEFT),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_MIDDLE),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_MIDDLE_RIGHT),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_LEFT_THUMB),
        INPUT_MAP_GAMEPAD_BUTTON(GAMEPAD_BUTTON_RIGHT_THUMB),
        // Triggers
        INPUT_MAP_GAMEPAD_TRIGGER(GAMEPAD_TRIGGER_LEFT),
        INPUT_MAP_GAMEPAD_TRIGGER(GAMEPAD_TRIGGER_RIGHT),
        INPUT_MAP_GAMEPAD_TRIGGER_NORM(GAMEPAD_TRIGGER_LEFT),
        INPUT_MAP_GAMEPAD_TRIGGER_NORM(GAMEPAD_TRIGGER_RIGHT),
        // Joysticks
        INPUT_MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_LEFT_X, JOYSTICK_DEATH_ZONE),
        INPUT_MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_LEFT_Y, JOYSTICK_DEATH_ZONE),
        INPUT_MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_RIGHT_X, JOYSTICK_DEATH_ZONE),
        INPUT_MAP_GAMEPAD_JOYSTICK(GAMEPAD_JOYSTICK_RIGHT_Y, JOYSTICK_DEATH_ZONE),
        // Zero'd the rest of the array
    };

    GreedyInputHandler handler = GreedyInputHandlerCreate(N_ACTIONS);
    GreedyInputHandlerMappingsSet(&handler, keyboard_mouse_locations, gamepad_locations);

    while (!WindowShouldClose()) {
        InputDeviceResults device_results = GreedyInputHandlerUpdate(&handler);
        bool is_keyboard_mouse = device_results.device == INPUT_DEVICE_ID_KEYBOARD_AND_MOUSE;
        InputMap *mappings = is_keyboard_mouse ? handler.keyboard_mouse_mappings : handler.gamepad_mappings;

        u32 x = TEXT_PADDING, y = TEXT_PADDING;

        BeginDrawing();
        ClearBackground(BLACK);

        DrawFPS(SCREEN_WIDTH - FPS_PADDING_X, FPS_PADDING_Y);
        draw_screen_text(font,
                         is_keyboard_mouse
                             ? "Device: Keyboard And Mouse"
                             : TextFormat("Device: Gamepad #%d - %s", device_results.device, GetGamepadName(device_results.device)),
                         x,
                         y);

        y += FONT_SIZE + TEXT_PADDING;

        for (input_size i = 0; i < N_ACTIONS; ++i) {
            InputResult result = device_results.results[i];
            if (result.bool_v) {
                InputMap map = mappings[i];
                String text;

                switch (map.method) {
                    case INPUT_METHOD_KEYBOARD_KEY: text = keyboard_key_text(map.input_id); break;
                    case INPUT_METHOD_MOUSE_BUTTON: text = mouse_button_text(map.input_id); break;
                    case INPUT_METHOD_MOUSE_POSITION: text = mouse_position_text(map.input_id); break;
                    case INPUT_METHOD_MOUSE_MOVEMENT: text = mouse_movement_text(map.input_id); break;
                    case INPUT_METHOD_MOUSE_SCROLL: text = mouse_scroll_text(map.input_id); break;

                    case INPUT_METHOD_GAMEPAD_BUTTON: text = gamepad_button_text(map.input_id); break;
                    case INPUT_METHOD_GAMEPAD_TRIGGER: text = gamepad_trigger_text(map.input_id); break;
                    case INPUT_METHOD_GAMEPAD_TRIGGER_NORM: text = gamepad_trigger_normalized_text(map.input_id); break;
                    case INPUT_METHOD_GAMEPAD_JOYSTICK: text = gamepad_joystick_text(map.input_id); break;

                    default: continue;
                }

                draw_screen_text(font, TextFormat("%s [b: %s, f: %.4f]", text.str, result.bool_v ? "true" : "false", result.float_v), x, y);
                y += FONT_SIZE + TEXT_SPACING;
            }
        }

        EndDrawing();
    }

    UnloadFont(font);

    return 0;
}