#include <raylib.h>
#include <fightable/renderer.h>
#include <fightable/state.h>
#include <fightable/intvec.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>
#include <fightable/editor.h>
#include <fightable/intro.h>
#include <math.h>
#include <stddef.h>
#include <stdio.h>

void _fDraw() {
    ClearBackground(BLACK);

    if (__state.intro_can_continue) {
        if (__state.current_editor != NULL) {
            _fEditorDraw(__state.current_editor);
        } else {
            if (__state.current_level) {
                _fLevelDraw(__state.current_level, (IVector2){0, 0});
            }
        }
    }

    Camera2D cam = {};
    cam.zoom = 1.f;
    cam.target = __state.gui_render_offset;

    BeginMode2D(cam);
    _fIntroDraw();
    EndMode2D();
}