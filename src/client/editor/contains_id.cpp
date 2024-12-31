#include <fightable/editor.h>
#include <fightable/editor.hpp>

unsigned char _fEditorContainsId(struct feditor *editor, unsigned short id) {
    if (!editor) return 0;

    int layer_id = editor->current_layer;
    if (layer_id < 0) layer_id = 0;

    for (auto &[x, ys] : editor->objects[layer_id]) {
        for (auto &[y, _obj] : ys) {
            if (_fBlockIdFromBlock(_obj) == id) {
                return 1;
            }
        }
    }

    return 0;
}