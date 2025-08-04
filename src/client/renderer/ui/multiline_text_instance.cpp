
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/text_instance.h"
#include <fightable/multiline_text_instance.hpp>
#include <fightable/string.h>
#include <string.h>
#include <fightable/generic_tools.hpp>

struct fmultiline_text_instance *_fMultilineTextInstanceCreateWithFont(const char *text, RLFont font, float size, float spacing) {
    struct fmultiline_text_instance *i = new fmultiline_text_instance;
    i->original_string = text;

    if (!text || !IsFontValid(font)) return i;

    auto lines = GenericTools::splitString(text, '\n');
    Vector2 next_offset = {};

    for (const auto &text_entry : lines) {
        struct fmultiline_text_instance_entry entry = {};

        entry.text = _fTextInstanceCreateWithFont(text_entry.c_str(), font, size, spacing);
        entry.offset = next_offset;

        Vector2 sz = _fTextInstanceGetSize(entry.text);
        next_offset.y += sz.y + spacing;

        i->lines.push_back(entry);
    }

    return i;
}
struct fmultiline_text_instance *_fMultilineTextInstanceCreateWithTextMan(const char *text, struct ftext_manager *man) {
    struct fmultiline_text_instance *i = new fmultiline_text_instance;
    i->original_string = text;

    if (!text || !man) return i;

    auto lines = GenericTools::splitString(text, '\n');
    Vector2 next_offset = {};

    for (const auto &text_entry : lines) {
        struct fmultiline_text_instance_entry entry = {};

        entry.text = _fTextInstanceCreateWithTextMan(text_entry.c_str(), man);
        entry.offset = next_offset;

        Vector2 sz = _fTextInstanceGetSize(entry.text);
        next_offset.y += sz.y + 1;

        i->lines.push_back(entry);
    }

    return i;
}

void _fMultilineTextInstanceDestroy(struct fmultiline_text_instance *instance) {
    if (!instance) return;

    for (const auto &e : instance->lines) {
        _fTextInstanceDestroy(e.text);
    }

    delete instance;
}

void _fMultilineTextInstanceDraw(struct fmultiline_text_instance *instance, Vector2 position) {
    if (!instance) return;

    for (const auto &entry : instance->lines) {
        Vector2 p = position;
        p.x += entry.offset.x;
        p.y += entry.offset.y;

        _fTextInstanceDraw(entry.text, p);
    }
}

Vector2 _fMultilineTextInstanceGetSize(struct fmultiline_text_instance *instance) {
    if (!instance) {}

    Vector2 vec = {};

    for (const auto &entry : instance->lines) {
        Vector2 sz = _fTextInstanceGetSize(entry.text);
        float height = entry.offset.y + sz.y;

        if (height > vec.y) {
            vec.y = height;
        }
        if (sz.x > vec.x) {
            vec.x = sz.x;
        }
    }

    return vec;
}

const char *_fMultilineTextInstanceGetText(struct fmultiline_text_instance *instance) {
    if (!instance) return NULL;

    return instance->original_string.c_str();
}

size_t _fMultilineTextInstanceGetLineAmount(struct fmultiline_text_instance *instance) {
    if (!instance) return 0;

    return instance->lines.size();
}
