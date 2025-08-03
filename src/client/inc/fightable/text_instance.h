
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <fightable/text.h>
#include <rsb/rsb_array_gen.h>

enum ftext_instance_command {
    TC_NONE = 0,
    TC_UNKNOWN,
    TC_COLOR_BASIC_TINT,
    TC_COLOR_GRADIENT
};

struct ftext_instance_entry {
    char *text;

    Texture2D prerendered_text;

    Color color_a;
    Color color_b;

    enum ftext_instance_command command;
};

RSB_ARRAY_DEF_GEN(struct ftext_instance_entry, _tie);

enum ftext_instance_backend {
    TIB_RAYLIB = 0,
    TIB_FIGHTABLE
};
struct ftext_instance {
    rsb_array__tie *text_elements;
    char *original_string;

    RLFont applicable_raylib_font;
    float raylib_size;
    float raylib_spacing;
    struct ftext_manager *applicable_fightable_font;

    enum ftext_instance_backend used_font_backend;
};

struct ftext_instance _fTextInstanceCreateWithFont(const char *text, RLFont font, float size, float spacing);
struct ftext_instance _fTextInstanceCreateWithTextMan(const char *text, struct ftext_manager *man);

void _fTextInstanceDestroy(struct ftext_instance *instance);
void _fTextInstanceDraw(struct ftext_instance *instance, Vector2 position);
Vector2 _fTextInstanceGetSize(struct ftext_instance *instance);

#ifdef __cplusplus
}
#endif
