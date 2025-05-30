
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "nt5emul/tui/environment.h"
#include "nt5emul/tui/file_selector.h"
#include "nt5emul/tui/menu.h"
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
#include <nt5emul/tui/text.h>
#include <nt5emul/tui/frame.h>
#include <nt5emul/tui/rectangle.h>

void _fDrawFileSelector(void *unused) {
    Color gray = (Color){ 30, 30, 30, 255 };

    RLRectangle r;

    struct nt_tui_environment *env = _ntGetTuiEnvironment();
    float sc = env->scaling;

    r.x = 0;
    r.y = 0;
    r.width = (int)((float)__state.overlay_framebuffer.texture.width / 8.f / sc + (1.f));
    r.height = (int)((float)__state.overlay_framebuffer.texture.height / 16 / sc + (1.f));

    _ntTuiDrawRectangleGr(r, BLACK, gray);

    r.x = 2;
    r.y = 2;
    r.width = (int)((float)__state.overlay_framebuffer.texture.width / 8 / sc - (6.f));
    r.height = (int)((float)__state.overlay_framebuffer.texture.height / 16 / sc - (4.f));

    _ntTuiDrawFrame(r, WHITE, NULL);
    _ntTuiDrawTextCentered("SELECT FILE", 0xFF, 1, WHITE);
    _ntTuiDrawMenu(__state.current_search_menu->base);
}

void _fDraw() {
    ClearBackground(BLACK);

    if (__state.current_search_menu) {
        _ntUpdateFileSelector(__state.current_search_menu);

        renderer_event_t ev;
        ev.user = NULL;
        ev.callback = _fDrawFileSelector;
        _fScheduleOverlayFunc(ev);
        return;
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
