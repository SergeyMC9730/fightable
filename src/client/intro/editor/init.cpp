#include <fightable/title_editor.h>
#include <fightable/title_editor.hpp>

struct ftitle_editor *_fTitleEditorCreate() {
    struct ftitle_editor *editor = new struct ftitle_editor;

    return editor;
}