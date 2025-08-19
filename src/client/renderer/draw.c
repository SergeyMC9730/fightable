
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
#include <fightable/mp_shared.h>

void _fDrawFileSelector(void *unused);

void _fDraw() {
#ifndef _DISABLE_MP_SERVER_
    _fMpTick();
#endif

    ClearBackground(BLACK);

    unsigned char fs_should_skip = 0;
    if (__state.current_search_menu) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            __state.current_search_menu->callback(__state.current_search_menu, NULL);
            _fCloseFileSelector();
        } else {
            _ntUpdateFileSelector(__state.current_search_menu);

            renderer_event_t ev;
            ev.user = NULL;
            ev.callback = _fDrawFileSelector;

            _fScheduleOverlayFunc(ev);

            return;
        }
    }

    if (__state.intro_can_continue) {
        if (__state.current_editor != NULL) {
            _fEditorDraw(__state.current_editor);
        }
    }

    Camera2D cam = {0};
    cam.zoom = 1.f;
    cam.target = __state.gui_render_offset;

    BeginMode2DStacked(cam);
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
    EndMode2DStacked();
}
