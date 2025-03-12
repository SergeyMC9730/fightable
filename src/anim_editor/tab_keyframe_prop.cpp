
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/anim_editor/keyframe_prop.hpp>
#include <fightable/color.h>

fightable::keyframe_prop_tab::keyframe_prop_tab() : tab({0, 512, 256, 720 - 512}, _fMixColors(_fMixColors(DARKGRAY, BLACK, 0.15f), BLACK, 0.15f), "Properties") {
    _verticalMovement = true;
    _limitScroll = true;
    _limitScrollSzHigh = {0, 500};
}

void fightable::keyframe_prop_tab::draw() {
    DrawRectangleRec({20, 20, 50, 60}, BLUE);
}
