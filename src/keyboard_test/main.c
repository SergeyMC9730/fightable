#include <raylib.h>
#include <stdint.h>
#include <string.h>

struct key_t {
    char first;
    char second;
};

/*
`~ 1! 2@ 3# 4$ 5% 6^ 7& 8* 9( 0) -_ =+ BACKSPACE
TAB q w e r t y u i o p [{ ]} \|
CAPS a s d f g h j k l ;/: '" ENTER
SHIFT z x c v b n m ,< .> /? SHIFT
CHANGELANG SPACE LEFTUP RIGHTDOWN PASTE HIDE
*/

enum special_keys_t {
    SPECIAL_BACKSPACE = 1,
    SPECIAL_TAB,
    SPECIAL_CAPS,
    SPECIAL_ENTER,
    SPECIAL_SHIFT,
    SPECIAL_CHANGE,
    SPECIAL_PASTE,
    SPECIAL_POS,

    SPECIAL_LAST
};

const char* GetSpecialKeyName(enum special_keys_t key) {
    switch (key) {
    case SPECIAL_BACKSPACE: return "Backspace";
    case SPECIAL_TAB: return "Tab";
    case SPECIAL_CAPS: return "Caps Lock";
    case SPECIAL_ENTER: return "Enter";
    case SPECIAL_SHIFT: return "Shift";
    case SPECIAL_CHANGE: return "Lang";
    case SPECIAL_PASTE: return "Paste";
    case SPECIAL_POS: return "Position";
    default: return NULL; break;
    }
}

const struct key_t keys[] = {
    // 14 keys
    {'`', '~'},
    {'1', '!'},
    {'2', '@'},
    {'3', '#'},
    {'4', '$'},
    {'5', '%'},
    {'6', '^'},
    {'7', '&'},
    {'8', '*'},
    {'9', '('},
    {'0', ')'},
    {'-', '_'},
    {'=', '+'},
    {SPECIAL_BACKSPACE, 0},

    // 14 keys
    {SPECIAL_TAB, 0},
    {'q', 0},
    {'w', 0},
    {'e', 0},
    {'r', 0},
    {'t', 0},
    {'y', 0},
    {'u', 0},
    {'i', 0},
    {'o', 0},
    {'p', 0},
    {'[', '{'},
    {']', '}'},
    {'\\', '|'},

    {SPECIAL_CAPS, 0},
    {'a', 0},
    {'s', 0},
    {'d', 0},
    {'f', 0},
    {'g', 0},
    {'h', 0},
    {'j', 0},
    {'k', 0},
    {'l', 0},
    {';', ':'},
    {'\'', '"'},
    {SPECIAL_ENTER},


    // 12 keys
    {SPECIAL_SHIFT, 0},
    {'z', 0},
    {'x', 0},
    {'c', 0},
    {'v', 0},
    {'b', 0},
    {'n', 0},
    {'m', 0},
    {'<', ','},
    {'>', '.'},
    {'/', '?'},
    {SPECIAL_SHIFT, 0},

    // 4 keys
    {SPECIAL_CHANGE},
    {' '},
    {SPECIAL_PASTE},
    {SPECIAL_POS}};

uint8_t key_count[] = {14, 14, 13, 12, 4};

typedef struct {
    bool shift;
    bool caps;
    bool draw_up;
} keyboard_t;

void DrawKeyboard(keyboard_t* keyboard, char* output);

int main() {
    InitWindow(1280, 720, "Keyboard test");
    SetWindowState(FLAG_WINDOW_RESIZABLE);

    keyboard_t keyboard = { 0 };

    char text[128] = { 0 };

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);
        RlDrawText(text, 0, 0, 20, BLACK);
        DrawKeyboard(&keyboard, text);
        EndDrawing();
    }

    return 0;
}

void DrawKeyboard(keyboard_t* keyboard, char* output) {
    const int screenWidth = GetScreenWidth();
    const int screenHeight = GetScreenHeight();
    const int padding = 3;
    const int fontheight = 40;
    const int rows = sizeof(key_count);

    // clang-format off
    const RLRectangle keyboard_bg = {
        0, screenHeight / 2.f * !keyboard->draw_up,
        screenWidth, screenHeight / 2.f
    };
    // clang-format on

    Color color = GRAY;

    DrawRectangleRec(keyboard_bg, ColorBrightness(color, -0.5f));

    const float key_height = keyboard_bg.height / rows - padding;
    Vector2 mouse = GetMousePosition();

    int i = 0;
    for (int y = 0; y < rows; y++) {
        const int colums = key_count[y];
        const float key_width = (float)screenWidth / colums - padding;

        for (int x = 0; x < key_count[y]; x++) {
            // clang-format off
            const RLRectangle key_rect = {
                keyboard_bg.x + padding * x + key_width * x,
                keyboard_bg.y + padding * y + key_height * y,
                key_width,
                key_height
            };
            // clang-format on

            Color keyColor = color;
            bool isKeyPressed = false;

            if(CheckCollisionPointRec(mouse, key_rect)) {
                keyColor = ColorBrightness(keyColor, (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) ? 0.75f : 0.5f);
                isKeyPressed = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
            }

            DrawRectangleRec(key_rect, keyColor);

            const bool hasSecondKey = keys[i].second;


            const char firstChar[2] = {(keyboard->shift && hasSecondKey) ? keys[i].second : keys[i].first};
            const char* first = (firstChar[0] && firstChar[0] < SPECIAL_LAST) ? GetSpecialKeyName(firstChar[0]) : firstChar;
            const int firstWidth = MeasureText(first, fontheight);

            const char secondChar[2] = {(keyboard->shift) ? keys[i].first : keys[i].second};
            const char* second = (secondChar[0] && secondChar[0] < SPECIAL_LAST) ? GetSpecialKeyName(secondChar[0]) : secondChar;
            const int secondHeight = MeasureText(second, fontheight / 2);

            RlDrawText(first, key_rect.x + (key_rect.width - firstWidth) / 2, key_rect.y + (key_rect.height - firstWidth) / 2, fontheight, WHITE);

            if (hasSecondKey) {
                RlDrawText(second, key_rect.x + key_rect.width - secondHeight - padding, key_rect.y + padding, fontheight / 2, RAYWHITE);
            }

            if (isKeyPressed && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                const char pressed = firstChar[0];
                const int length = strlen(output);

                switch (pressed) {
                    case SPECIAL_BACKSPACE:
                        output[length - 1] = '\0';
                        break;
                    case SPECIAL_TAB:
                        output[length] = '\t';
                        break;
                    case SPECIAL_CAPS:
                        keyboard->caps ^= 1;
                        break;
                    case SPECIAL_ENTER:
                        output[length] = '\n';
                        break;
                    case SPECIAL_SHIFT:
                        keyboard->shift ^= 1;
                        break;
                    case SPECIAL_CHANGE:
                        // fuck off
                        break;
                    case SPECIAL_PASTE:
                        const char* clipboard = GetClipboardText();

                        if (clipboard) {
                            strcpy(output + length, clipboard);
                        }

                        break;
                    case SPECIAL_POS:
                        keyboard->draw_up ^= 1;
                        break;

                    default:
                        output[length] = pressed - 32 * (keyboard->shift && pressed >= 'a');

                        if (keyboard->shift && !keyboard->caps) {
                            keyboard->shift = false;
                        }

                        break;
                }
            }

            i++;
        }
    }
}
