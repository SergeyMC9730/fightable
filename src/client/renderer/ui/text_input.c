
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/multiline_text_instance.h"
#include "fightable/string.h"
#include "raylib.h"
#include <fightable/text_input.h>
#include <fightable/renderer.h>
#include <string.h>
#include <stdio.h>
#include <fightable/state.h>
#include <sys/types.h>

#define INPUT_SAFE_ZONE 10

unsigned int _fTextInputIsRestricted(struct ftext_input *input, const char *c) {
    return input != NULL && c != NULL && input->restricted != NULL && strstr(c, input->restricted) != NULL;
}

void _fTextInputAppendCodepoint(struct ftext_input *input, int codepoint) {
    if (!input) return;

    int utf_size = 0;
    const char *utf_char = CodepointToUTF8(codepoint, &utf_size);

    if (!utf_char || utf_char <= 0 || _fTextInputIsRestricted(input, utf_char)) {
        return;
    }
    if (input->characters_entered >= input->max_characters || strlen(input->buffer) + utf_size >= (input->buffer_length - 1)) {
        return;
    }

    unsigned int o = _fGetUtf8ByteOffset(input->buffer, input->pointer_char_index);
    char *offset = input->buffer + o;
    memmove(offset + utf_size, offset, input->buffer_length - utf_size);
    memcpy(offset, utf_char, utf_size);

    input->buffer[input->buffer_length - 1] = 0;

    input->characters_entered++;
    input->pointer_char_index++;
    input->pointer_pos.x += MeasureTextEx(input->rl_font, utf_char, input->rl_size, input->rl_spacing).x + input->rl_spacing;
}
void _fTextInputPopChar(struct ftext_input *input) {
    if (!input || input->characters_entered == 0 || input->pointer_char_index <= 0) return;

    int i = input->pointer_char_index - 1;

    int codepoint = _fGetUtf8AtIndex(input->buffer, i);
    int utf_size = 0;
    CodepointToUTF8(codepoint, &utf_size);

    if (utf_size <= 0) return;

    unsigned int o = _fGetUtf8ByteOffset(input->buffer, i);
    char *offset = input->buffer + o;

    char symbol[6] = {};
    memcpy(symbol, offset, utf_size);

    TraceLog(LOG_INFO, "%s", symbol);

    memmove(offset, offset + utf_size, input->buffer_length - utf_size);

    input->characters_entered--;
    input->pointer_char_index--;
    input->pointer_pos.x -= MeasureTextEx(input->rl_font, symbol, input->rl_size, input->rl_spacing).x + input->rl_spacing;
}
void _fTextInputUpdateLabel(struct ftext_input *input) {
    if (!input) return;

    if (input->rendered_text) {
        _fMultilineTextInstanceDestroy(input->rendered_text);
        input->rendered_text = NULL;
    }

    char *formatted_str = (char *)MemAlloc(input->buffer_length + 10);
    snprintf(formatted_str, input->buffer_length + 10, "<cblack>%s", input->buffer);

    input->rendered_text = _fMultilineTextInstanceCreateWithFont(formatted_str, input->rl_font, input->rl_size, input->rl_spacing);
    input->rendered_text_size = _fMultilineTextInstanceGetSize(input->rendered_text);
    input->box.height = INPUT_SAFE_ZONE + input->rendered_text_size.y;

    if (input->box.height == INPUT_SAFE_ZONE) {
        input->box.height += input->rl_size;
    }

    MemFree(formatted_str);
}

struct ftext_input *_fTextInputCreate(unsigned int max_characters, const char *restricted_chars, RLFont font, float size, float spacing, int input_width_fb, Vector2 input_pos_fb) {
    if (max_characters == 0 || !IsFontValid(font)) return NULL;

    struct ftext_input *obj = (struct ftext_input *)MemAlloc(sizeof(struct ftext_input));
    obj->buffer_length = (max_characters * 4) + 1; // allocate enough space for unicode characters
    obj->buffer = (char *)MemAlloc(sizeof(char) * obj->buffer_length);
    obj->box.width = _fPosFramebufferToOverlay((Vector2){input_width_fb, 0}).x;
    obj->box.height = INPUT_SAFE_ZONE + size;
    obj->restricted = restricted_chars;
    input_pos_fb = _fPosFramebufferToOverlay(input_pos_fb);
    obj->box.x = input_pos_fb.x;
    obj->box.y = input_pos_fb.y;
    obj->rl_font = font;
    obj->rl_size = size;
    obj->rl_spacing = spacing;
    obj->max_characters = max_characters;

    return obj;
}

void _fTextInputUpdate(struct ftext_input *input) {
    if (!input) return;

    if (input->pointer_char_index < 0) {
        input->pointer_char_index = 0;
    }
    if (input->pointer_pos.x < 0) {
        input->pointer_pos.x = 0;
    }

    Vector2 mouse = _fGetMousePosOverlay();

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        input->selected = CheckCollisionPointRec(mouse, input->box);
    }

    if (!input->selected) return;

    if (_fKeyPressedR(KEY_BACKSPACE)) {
        _fTextInputPopChar(input);
        _fTextInputUpdateLabel(input);

        TraceLog(LOG_INFO, "New buffer data (pop): %s", input->buffer);

        return;
    }

    int codepoint = 0;
    if (_fKeyPressedR(KEY_ENTER)) {
        codepoint = '\n';
    } else {
        codepoint = GetCharPressed();
    }
    while (codepoint) {
        _fTextInputAppendCodepoint(input, codepoint);
        _fTextInputUpdateLabel(input);

        TraceLog(LOG_INFO, "New buffer data (append): %s", input->buffer);

        codepoint = GetCharPressed();
    }

    if (_fKeyPressedR(KEY_LEFT)) {
        if (input->pointer_char_index != 0) {
            input->pointer_char_index--;

            int i = input->pointer_char_index;

            int utf_size = 0;
            const char *symbol = CodepointToUTF8(_fGetUtf8AtIndex(input->buffer, i), &utf_size);
            if (utf_size > 0) {
                TraceLog(LOG_INFO, "LEFT: %s (%d; %d)", symbol, utf_size, i);
                input->pointer_pos.x -= MeasureTextEx(input->rl_font, symbol, input->rl_size, input->rl_spacing).x + input->rl_spacing;
            } else {
                TraceLog(LOG_ERROR, "Unknown char at %d (sz=%d)", i, utf_size);
            }
        }
    } else if (_fKeyPressedR(KEY_RIGHT)) {
        if (input->pointer_char_index < input->characters_entered) {
            input->pointer_char_index++;

            int i = input->pointer_char_index;
            if (input->pointer_char_index >= input->characters_entered) {
                i--;
            }
            int utf_size = 0;
            const char *symbol = CodepointToUTF8(_fGetUtf8AtIndex(input->buffer, i), &utf_size);
            if (utf_size > 0) {
                TraceLog(LOG_INFO, "RIGHT: %s (%d; %d)", symbol, utf_size, i);
                input->pointer_pos.x += MeasureTextEx(input->rl_font, symbol, input->rl_size, input->rl_spacing).x + input->rl_spacing;
            } else {
                TraceLog(LOG_ERROR, "Unknown char at %d (sz=%d)", i, utf_size);
            }
        } else {
            TraceLog(LOG_ERROR, "Array check failure: %d %u", input->pointer_char_index, input->characters_entered);
        }
    }

    // TraceLog(LOG_INFO, "%d %f %u", input->pointer_char_index, input->pointer_pos.x, input->characters_entered);
}
void _fTextInputQueueRenderer(struct ftext_input *input) {
    if (!input) return;

    Vector2 shake_conv = _fPosFramebufferToOverlay(__state.gui_render_offset);
    RLRectangle tr = input->box;
    int h = INPUT_SAFE_ZONE / 2;

    tr.x += shake_conv.x;
    tr.y += shake_conv.y;

    DrawRectangleRec(tr, WHITE);
    if (input->selected) {
        DrawRectangleLinesEx(tr, (float)INPUT_SAFE_ZONE / 4.f, __state.intro_text_tint);
    }

    if (input->rendered_text == NULL) return;

    Vector2 text_size = input->rendered_text_size;
    text_size = input->pointer_pos;
    if (text_size.x > (tr.width - INPUT_SAFE_ZONE)) {
        BeginScissorMode(tr.x + h, tr.y + h, tr.width - (h * 2), tr.height - h);
        _fMultilineTextInstanceDraw(input->rendered_text, (Vector2){tr.x - text_size.x + tr.width - h, tr.y + h});
        EndScissorMode();
        if (input->selected) {
            DrawRectangle(tr.x + tr.width - h, tr.y + h, 4, tr.height - (h * 2), GRAY);
        }
    } else {
        _fMultilineTextInstanceDraw(input->rendered_text, (Vector2){tr.x + h, tr.y + h});
        if (input->selected) {
            DrawRectangle(tr.x + input->pointer_pos.x + h, tr.y + input->pointer_pos.y + h, 4, tr.height - (h * 2), GRAY);
        }
    }
}
void _fTextInputRenderText(struct ftext_input *input) {
    renderer_event_t event;

    event.callback = REVENT_CALLBACK_CAST(_fTextInputQueueRenderer);
    event.user = input;

    _fScheduleOverlayFunc(event);
}

void _fTextInputDestroy(struct ftext_input *input) {
    if (!input) return;

    MemFree(input->buffer);
    if (input->rendered_text) _fMultilineTextInstanceDestroy(input->rendered_text);

    MemFree(input);
}
