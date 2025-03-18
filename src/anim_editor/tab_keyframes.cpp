
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/anim_editor/keyframe.hpp>
#include <fightable/color.h>

fightable::keyframe_tab::keyframe_tab() : tab({256, 512, 1280 - 256, 720 - 512}, _fMixColors(DARKGRAY, BLACK, 0.15f), "Keyframes") {
    _horizontalMovement = true;
    _verticalMovement = true;
    _limitScroll = true;
    _limitScrollSzHigh = {500, 500};
}

void fightable::keyframe_tab::draw() {
    // Vector2 m = getMousePosInCamera();
    // DrawRectangleRec({m.x, m.y, 60, 60}, GREEN);
}
