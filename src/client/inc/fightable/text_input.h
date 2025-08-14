
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <nt5emul/renderer_animation.h>
#include <nt5emul/renderer_keyframe.h>
#include <fightable/multiline_text_instance.h>

#ifdef __cplusplus
extern "C" {
#endif

struct ftext_input_osk {
    unsigned char shift;
    unsigned char caps;
    unsigned char draw_up;

    struct renderer_animation cur_animation;
    struct renderer_keyframe  cur_keyframe;
};
struct ftext_input {
    char *buffer;
    unsigned int buffer_length;
    unsigned int characters_entered;
    unsigned int max_characters;
    struct fmultiline_text_instance *rendered_text;
    Vector2 rendered_text_size;

    const char *restricted;

    struct ftext_input_osk mob_osk;
    unsigned char mob_moved_on_touch;
    unsigned char selected;
    unsigned char locked;

    Vector2 pointer_pos;
    int pointer_char_index;

    RLRectangle box;

    RLFont rl_font;
    float rl_size;
    float rl_spacing;
};

struct ftext_input *_fTextInputCreate(unsigned int max_characters, const char *restricted_chars, RLFont font, float size, float spacing, int input_width_fb, Vector2 input_pos_fb);

void _fTextInputUpdate(struct ftext_input *input);
void _fTextInputQueueRenderer(struct ftext_input *input);
void _fTextInputRenderText(struct ftext_input *input);
void _fTextInputDestroy(struct ftext_input *input);
void _fTextInputLock(struct ftext_input *input);
void _fTextInputUnlock(struct ftext_input *input);
unsigned char _fTextInputEmpty(struct ftext_input *input);

#ifdef __cplusplus
}
#endif
