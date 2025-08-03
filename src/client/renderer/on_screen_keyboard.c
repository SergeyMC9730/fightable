
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <fightable/state.h>

struct fosk_row {
    RLRectangle row_keys[14];
    RLRectangle row_keys_sized[14];
    char row_chars[14][2];
    unsigned char row_state[14];
};

void _fOskScaleRow(RLRectangle *row, unsigned int buttons, Vector2 expected_size) {
    if (!row || buttons == 0) return;
    if (expected_size.x * expected_size.y == 0.f) return;

    float orig_width = 0;
    float orig_height = 0;
    for (unsigned int i = 0; i < buttons; i++) {
        orig_width += row[i].width;
        if (row[i].height > orig_height) {
            orig_height = row[i].height;
        }
    }

    if (orig_width * orig_height == 0.f) return;

    float width_ratio = expected_size.x / orig_width;
    float height_ratio = expected_size.y / orig_height;

    for (unsigned int i = 0; i < (buttons - 1); i++) {
        RLRectangle *r = row + i;

        float old_w = r->width;

        r->width *= width_ratio;
        r->height *= height_ratio;

        float w_delta = r->width - old_w;

        r++;
        r->x += w_delta;
    }
}
Vector2 _fOskGetKeyboardSize() {
    float height_ratio = 0.4;
    Vector2 screen_size = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    float keyboard_height = screen_size.y * height_ratio;

    return (Vector2){screen_size.x, keyboard_height};
}
void _fOskDrawRow(struct fosk_row *r) {
    Vector2 sz = _fOskGetKeyboardSize();
    Vector2 screen_size = {(float)GetScreenWidth(), (float)GetScreenHeight()};

    for (unsigned int i = 0; i < 14; i++) {
        if (r->row_chars[i][0] == 0) continue;

        r->row_keys_sized[i] = r->row_keys[i];
        _fOskScaleRow(r->row_keys_sized + i, 14, sz);

        RLRectangle real_rect = r->row_keys_sized[i];
        real_rect.y = screen_size.y - real_rect.y - real_rect.height;

        // sTraceLog(LOG_INFO, "(%d) %f %f %f %f", i, real_rect.x, real_rect.y, real_rect.width, real_rect.height);

        DrawRectangleRec(real_rect, RAYWHITE);
        DrawRectangleLinesEx(real_rect, 2.f, BLACK);
    }
}

void _fDrawOnScreenKeyboard() {
    float height_ratio = 0.4;
    Vector2 screen_size = {(float)GetScreenWidth(), (float)GetScreenHeight()};
    float keyboard_height = screen_size.y * height_ratio;

    DrawRectangle(0, screen_size.y - keyboard_height, screen_size.x, keyboard_height, (Color){0,0,0,200});

    for (unsigned int i = 0; i < __state.osk_rows_size; i++) {
        _fOskDrawRow(__state.osk_rows + i);
    }
}

void _fOpenOnScreenKeyboard(RLRectangle input_field, char *buffer, unsigned int buffer_size, renderer_event_t on_complete) {
    if (!buffer || buffer_size == 0) return;

    __state.osk_buffer = buffer;
    __state.osk_buffer_size = buffer_size;
    __state.osk_input_field = input_field;
    __state.osk_callback = on_complete;

    if (__state.osk_rows) {
        free(__state.osk_rows);
    }
    __state.osk_rows_size = 5;
    __state.osk_rows = (struct fosk_row *)malloc(sizeof(struct fosk_row) * __state.osk_rows_size);
    memset(__state.osk_rows, 0, sizeof(struct fosk_row) * __state.osk_rows_size);

#define BK3(I, A, B, Y, OX, W) r->row_keys[I] = (RLRectangle){0 + OX, Y, W, 1}; r->row_chars[I][0] = '`'; r->row_chars[I][1] = '~'; if (I > 0) { r->row_keys[I].x += r->row_keys[I - 1].x + r->row_keys[I - 1].width; }
#define BK2(I, A, B, Y) BK3(I, A, B, Y, 0, 1)
#define  BK(I, A, B) BK2(I, A, B, 0)

    {
        struct fosk_row *r = __state.osk_rows + 0;
        BK(0 , '`', '~');
        BK(1 , '1', '!');
        BK(2 , '2', '@');
        BK(3 , '3', '#');
        BK(4 , '4', '$');
        BK(5 , '5', '%');
        BK(6 , '6', '^');
        BK(7 , '7', '&');
        BK(8 , '8', '*');
        BK(9 , '9', '(');
        BK(10, '0', ')');
        BK(11, '-', '_');
        BK(12, '=', '+');
        BK3(13, 0x01, 0x01, 0, 0, 2);
        return;
    }

    {
        struct fosk_row *r = __state.osk_rows + 1;
        BK3(0, 0x02, 0x02, 0, 0, 2);
        BK(1 , 'q', 'Q');
        BK(2 , 'w', 'W');
        BK(3 , 'e', 'R');
        BK(4 , 'r', 'R');
        BK(5 , 't', 'T');
        BK(6 , 'y', 'Y');
        BK(7 , 'u', 'U');
        BK(8 , 'i', 'I');
        BK(9 , 'o', 'O');
        BK(10, 'p', 'P');
        BK(11, '[', '{');
        BK(12, ']', ']');
        BK(13, '\\', '|');
    }

    {
        struct fosk_row *r = __state.osk_rows + 2;
        BK3(0, 0x03, 0x03, 0, 0, 2);
        BK(1 , 'a', 'A');
        BK(2 , 's', 'S');
        BK(3 , 'd', 'D');
        BK(4 , 'f', 'F');
        BK(5 , 'g', 'G');
        BK(6 , 'h', 'H');
        BK(7 , 'j', 'J');
        BK(8 , 'k', 'K');
        BK(9 , 'l', 'L');
        BK(10, ';', ':');
        BK(11, '\'', '"');
        BK3(12, 0x04, 0x04, 0, 0, 2);
    }

    {
        struct fosk_row *r = __state.osk_rows + 3;
        BK3(0, 0x04, 0x04, 0, 0, 2.5);
        BK(1 , 'z', 'Z');
        BK(2 , 'x', 'X');
        BK(3 , 'c', 'C');
        BK(4 , 'v', 'V');
        BK(5 , 'b', 'B');
        BK(6 , 'n', 'N');
        BK(7 , 'm', 'M');
        BK(8 , ',', '<');
        BK(9 , '.', '>');
        BK(10, '/', '?');
        BK3(11, 0x04, 0x04, 0, 0, 2.5);
    }

    {
        struct fosk_row *r = __state.osk_rows + 4;
        BK3(0, 0x05, 0x05, 0, 0, 1.5);
        BK3(1, 0x06, 0x06, 0, 0, 0.8);
        BK3(2, 0x07, 0x07, 0, 0, 1.5);
        BK3(3, ' ', ' ', 0, 0, 6.6);
        BK3(4, 0x07, 0x07, 0, 0, 1.5);
        BK3(5, 0x06, 0x06, 0, 0, 0.8);
        BK3(6, 0x08, 0x08, 0, 0, 0.8);
        BK3(7, 0x05, 0x05, 0, 0, 1.5);
    }

#undef BK
#undef BK2
#undef BK3

    for (unsigned int i = 0; i < __state.osk_rows_size; i++) {
        struct fosk_row *r = __state.osk_rows + i;
        for (unsigned int j = 0; j < 14; j++) {
            r->row_keys[j].y = i;
        }
    }
}
void _fCloseOnScreenKeyboard() {

}
