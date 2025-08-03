
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/multiline_text_instance.h>
#include <fightable/string.h>

struct fmultiline_text_instance _fMultilineTextInstanceCreateWithFont(const char *text, RLFont font, float size, float spacing) {
    struct fmultiline_text_instance i = {};

    if (!text || !IsFontValid(font)) return i;

    rsb_array__pchar *lines = _fSplitString(text, '\n');

    i.line_amount = lines->added_elements;
    i.lines = (struct fmultiline_text_instance_entry *)MemAlloc(sizeof(struct fmultiline_text_instance_entry));

    Vector2 next_offset = {};

    for (unsigned int ii = 0; ii < i.line_amount; ii++) {
        char *text_entry = lines->objects[ii];
        TraceLog(LOG_INFO, "GOVNO: %s", (const char *)text_entry);
        struct fmultiline_text_instance_entry *entry = i.lines + ii;

        entry->text = _fTextInstanceCreateWithFont(text_entry, font, size, spacing);
        entry->offset = next_offset;

        Vector2 sz = _fTextInstanceGetSize(&entry->text);
        next_offset.y += sz.y + spacing;
    }

    // _fCleanupSplittedString(lines);

    return i;
}
struct fmultiline_text_instance _fMultilineTextInstanceCreateWithTextMan(const char *text, struct ftext_manager *man) {
    struct fmultiline_text_instance i = {};

    if (!text || !man) return i;

    rsb_array__pchar *lines = _fSplitString(text, '\n');

    i.line_amount = lines->added_elements;
    i.lines = (struct fmultiline_text_instance_entry *)MemAlloc(sizeof(struct fmultiline_text_instance_entry));

    Vector2 next_offset = {};

    for (unsigned int ii = 0; ii < i.line_amount; ii++) {
        char *text_entry = lines->objects[ii];
        struct fmultiline_text_instance_entry *entry = i.lines + ii;

        entry->text = _fTextInstanceCreateWithTextMan(text_entry, man);
        entry->offset = next_offset;

        Vector2 sz = _fTextInstanceGetSize(&entry->text);
        next_offset.y += sz.y + 1;
    }

    _fCleanupSplittedString(lines);

    return i;
}

void _fMultilineTextInstanceDestroy(struct fmultiline_text_instance *instance) {
    if (!instance || !instance->lines) return;

    for (unsigned int i = 0; i < instance->line_amount; i++) {
        _fTextInstanceDestroy(&(instance->lines + i)->text);
    }

    MemFree(instance->lines);
    instance->lines = NULL;
    instance->line_amount = 0;
}

void _fMultilineTextInstanceDraw(struct fmultiline_text_instance *instance, Vector2 position) {
    if (!instance || !instance->lines) return;

    for (unsigned int i = 0; i < instance->line_amount; i++) {
        struct fmultiline_text_instance_entry *entry = instance->lines + i;

        Vector2 p = position;
        p.x += entry->offset.x;
        p.y += entry->offset.y;

        _fTextInstanceDraw(&entry->text, p);
    }
}

Vector2 _fMultilineTextInstanceGetSize(struct fmultiline_text_instance *instance) {
    if (!instance || !instance->lines) {}
    return (Vector2){};
}
