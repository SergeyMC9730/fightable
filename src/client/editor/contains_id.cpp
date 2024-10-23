#include <fightable/editor.h>
#include <fightable/editor.hpp>

unsigned char _fEditorContainsId(struct feditor *editor, unsigned short id) {
    if (!editor) return 0;

    for (auto &[x, ys] : editor->objects) {
        for (auto &[y, _obj] : ys) {
            if (_fBlockIdFromBlock(_obj) == id) {
                return 1;
            }
        }
    }

    return 0;
}