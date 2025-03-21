
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <fightable/state.h>
#include <fightable/intvec.h>
#include <fightable/tilemap.h>
#include <fightable/level.h>
#include <fightable/editor.h>
#include <fightable/intro.h>
#include <fightable/mp_create_menu.h>
#include <stddef.h>
#include <stdio.h>

void _fDraw() {
    ClearBackground(BLACK);

    if (__state.intro_can_continue) {
        if (__state.current_editor != NULL) {
            _fEditorDraw(__state.current_editor);
        } else {
            if (__state.current_level && __state.current_ui_menu != UI_MENU_MPCREATE) {
                _fLevelDraw(__state.current_level, (IVector2){0, 0});
            }
        }
    }

    Camera2D cam = {0};
    cam.zoom = 1.f;
    cam.target = __state.gui_render_offset;

    BeginMode2D(cam);
    switch (__state.current_ui_menu) {
    case UI_MENU_MAIN: {
        _fIntroDraw();
        break;
    }
    case UI_MENU_MPCREATE: {
        _fMpCreateDraw();
        break;
    }
    }
    EndMode2D();
}
