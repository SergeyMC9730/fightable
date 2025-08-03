//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <fightable/text_instance.h>

struct fmultiline_text_instance_entry {
    struct ftext_instance text;
    Vector2 offset;
};
struct fmultiline_text_instance {
    struct fmultiline_text_instance_entry *lines;
    unsigned int line_amount;
};

struct fmultiline_text_instance _fMultilineTextInstanceCreateWithFont(const char *text, RLFont font, float size, float spacing);
struct fmultiline_text_instance _fMultilineTextInstanceCreateWithTextMan(const char *text, struct ftext_manager *man);

void _fMultilineTextInstanceDestroy(struct fmultiline_text_instance *instance);
void _fMultilineTextInstanceDraw(struct fmultiline_text_instance *instance, Vector2 position);
Vector2 _fMultilineTextInstanceGetSize(struct fmultiline_text_instance *instance);

#ifdef __cplusplus
}
#endif
