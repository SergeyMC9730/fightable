
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "fightable/text_input.h"
#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/button.h>
#include <fightable/flags.h>
#include <fightable/slider.h>

#define SETUP_BUTTON(NAME, Y, XO) SETUP_BUTTON_EX(NAME, Y, WHITE, XO)
#define SETUP_BUTTON_EX(NAME, Y, COLOR, XO) sz = _fButtonMeasureSizeSimple(NAME); if (_fButtonDrawSimple(NAME, (IVector2){(wx - sz) / 2 + __state.menu_cur_x + (float)(XO), Y}, COLOR))

void _fIntroMenuProcessMultiplayerJoin() {
    int wx = __state.framebuffer.texture.width;
    int wy = __state.framebuffer.texture.height;

    DrawTexture(__state.playbtn_container, 0, 0, WHITE);

    _fTextInputUpdate(__state.ip_input);
    _fTextInputRenderText(__state.ip_input);

    _fTextInputUpdate(__state.name_input);
    _fTextInputRenderText(__state.name_input);

    unsigned int sz = 0;

    SETUP_BUTTON("Join", 68, 45) {

    }
}
