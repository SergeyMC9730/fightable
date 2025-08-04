
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/intro.h>
#include <fightable/state.h>
#include <fightable/button.h>
#include <fightable/flags.h>
#include <fightable/slider.h>

void _fIntroMenuProcessMultiplayerJoin() {
    int wxx = __state.framebuffer.texture.width;
    int wyy = __state.framebuffer.texture.height;

    DrawTexture(__state.playbtn_container, 0, 0, WHITE);
}
