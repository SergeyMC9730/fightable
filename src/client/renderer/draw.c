#include <raylib.h>
#include <fightable/renderer.h>
#include <fightable/state.h>
#include <fightable/intvec.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>
#include <fightable/editor.h>
#include <math.h>
#include <stddef.h>

void _fDraw() {
    ClearBackground(DARKGRAY);

    if (__state.current_editor != NULL) {
        _fEditorDraw(__state.current_editor);
    } else {
        if (__state.current_level) {
            _fLevelDraw(__state.current_level, (IVector2){0, 0});
        }
    }
}