
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/renderer.h>
#include <fightable/state.h>

Vector2 _fGetCurrentFramebufferSize() {
    Vector2 sz = {};

    if (_fSchedulerInOverlay()) {
        sz.x = (float)__state.overlay_framebuffer.texture.width;
        sz.y = (float)__state.overlay_framebuffer.texture.height;
    } else {
        sz.x = (float)__state.framebuffer.texture.width;
        sz.y = (float)__state.framebuffer.texture.height;
    }

    return sz;
}
