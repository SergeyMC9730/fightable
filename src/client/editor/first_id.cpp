
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/editor.h>
#include <fightable/editor.hpp>

IVector2 _fEditorGetPosOfFirstId(struct feditor *editor, unsigned short id) {
    if (!editor) return {};

    for (auto &obj : editor->render_objects) {
        if (_fBlockIdFromRenderable(obj.base) == id) {
            return {obj.base.block_x, obj.base.block_y};
        }
    }

    return {};
}
IVector2 _fEditorGetPosOfFirstIdLvl(struct flevel *level, unsigned short id) {
    if (!level) return {};

    for (unsigned long i = 0; i < level->data_size; i++) {
        fblock *block = level->objects + i;
        if (_fBlockIdFromRenderable(block->base) == id) {
            return {block->base.block_x, block->base.block_y};
        }
    }

    return {};
}
