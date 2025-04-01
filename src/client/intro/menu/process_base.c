
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/intro.h>
#include <fightable/intvec.h>
#include <fightable/state.h>
#include <stdlib.h>
#include <fightable/button.h>

void _fIntroMenuProcessBase() {
#define SETUP_BUTTON(NAME, Y) SETUP_BUTTON_EX(NAME, Y, WHITE)
#define SETUP_BUTTON_EX(NAME, Y, COLOR) sz = _fButtonMeasureSizeSimple(NAME); if (_fButtonDrawSimple(NAME, (IVector2){(wx - sz) / 2 + __state.menu_cur_x, Y}, COLOR))
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;
    int sz = 0;

    SETUP_BUTTON("PLAY", 50) {
        _fIntroMenuOnPlay();
    }
    SETUP_BUTTON("OPTIONS", 60) {
        _fIntroMenuOnOptions();
    }
    SETUP_BUTTON("EDITOR", 70) {
        _fIntroMenuOnEditor();
    }
    SETUP_BUTTON("EXIT", 80) {
        _fConfigSave(&__state.config);
        exit(0);
    }
}
