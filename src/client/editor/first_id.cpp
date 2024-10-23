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