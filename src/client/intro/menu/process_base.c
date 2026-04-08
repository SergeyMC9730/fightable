
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/intro.h>
#include <fightable/intvec.h>
#include <fightable/state.h>
#include <stdlib.h>
#include <fightable/button.h>

void _fIntroMenuProcessBase() {
#define SETUP_BUTTON(NAME, Y, ALT) SETUP_BUTTON_EX(NAME, Y, WHITE, ALT)
#define SETUP_BUTTON_EX(NAME, Y, COLOR, ALT) sz = _fButtonMeasureSizeSimple(NAME); if (_fButtonDrawSimple(NAME, (IVector2){(wx - sz) / 2 + __state.menu_cur_x, Y}, COLOR, ALT))
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;
    int sz = 0;

    SETUP_BUTTON("PLAY", 50, "menu.base.play") {
        _fIntroMenuOnPlay();
    }
    SETUP_BUTTON("OPTIONS", 60, "menu.base.options") {
        _fIntroMenuOnOptions();
    }
    SETUP_BUTTON("EDITOR", 70, "menu.base.editor") {
        _fIntroMenuOnEditor();
    }
    SETUP_BUTTON("EXIT", 80, "menu.base.exit") {
        _fConfigSave(&__state.config);
        exit(0);
    }
}
