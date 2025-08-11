
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
#include <math.h>
#include <fightable/state.h>

#define INPUT_SAFE_ZONE 10

unsigned int _fTextInputIsRestricted(struct ftext_input *input, const char *c) {
    return input != NULL && c != NULL && input->restricted != NULL && strstr(c, input->restricted) != NULL;
}

void _fTextInputAppendCodepoint(struct ftext_input *input, int codepoint) {
    if (!input) return;

    int utf_size = 0;
    const char *utf_char = CodepointToUTF8(codepoint, &utf_size);

    if (!utf_char || utf_char <= 0 || _fTextInputIsRestricted(input, utf_char)) return;
    if (strlen(input->buffer) + utf_size >= (input->buffer_length - 1)) return;

    strncat(input->buffer, utf_char, utf_size);
    input->buffer[input->buffer_length - 1] = 0;

    input->characters_entered++;
}
void _fTextInputPopChar(struct ftext_input *input) {
    if (!input || input->characters_entered == 0) return;

    int codepoint = _fGetUtf8AtIndex(input->buffer, input->characters_entered - 1);
    int utf_size = 0;
    CodepointToUTF8(codepoint, &utf_size);

    if (utf_size <= 0) return;

    input->buffer[strlen(input->buffer) - utf_size] = 0;
    input->characters_entered--;
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

    TraceLog(LOG_INFO, "Updated text with: %s\nINFO: Lines: %ld", formatted_str, _fMultilineTextInstanceGetLineAmount(input->rendered_text));

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

    return obj;
}

void _fTextInputUpdate(struct ftext_input *input) {
    if (!input) return;

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

    }
}
void _fTextInputQueueRenderer(struct ftext_input *input) {
    if (!input) return;

    DrawRectangleRec(input->box, WHITE);
    if (input->selected) DrawRectangleLinesEx(input->box, (float)INPUT_SAFE_ZONE / 4.f, __state.intro_text_tint);

    if (input->rendered_text == NULL) return;

    Vector2 text_size = input->rendered_text_size;
    if (text_size.x > (input->box.width - INPUT_SAFE_ZONE)) {
        // TraceLog(LOG_INFO, "Scissor");

        int h = INPUT_SAFE_ZONE / 2;
        BeginScissorMode(input->box.x + h, input->box.y + h, input->box.width - (h * 2), input->box.height - h);

        _fMultilineTextInstanceDraw(input->rendered_text, (Vector2){input->box.x - text_size.x + (input->box.width / 2), input->box.y + h});

        EndScissorMode();
    } else {
        // TraceLog(LOG_INFO, "Default %d", _fMultilineTextInstanceGetLineAmount(input->rendered_text));

        int h = INPUT_SAFE_ZONE / 2;
        _fMultilineTextInstanceDraw(input->rendered_text, (Vector2){input->box.x + h, input->box.y + h});
    }
}
void _fTextInputRenderText(struct ftext_input *input) {
    renderer_event_t event;

    event.callback = REVENT_CALLBACK_CAST(_fTextInputQueueRenderer);
    event.user = input;

    _fScheduleOverlayFunc(event);
}
