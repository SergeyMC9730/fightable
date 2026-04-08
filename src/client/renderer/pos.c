
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include "raylib.h"
#include <fightable/renderer.h>
#include <fightable/state.h>

Vector2 _fPosScreenToFramebuffer(Vector2 pos) {
    pos.x /= __state.window_scale;
    pos.y /= __state.window_scale;
    return pos;
}
Vector2 _fPosScreenToOverlay(Vector2 pos) {
    pos = _fPosScreenToFramebuffer(pos);
    pos.x *= UI_SCALE;
    pos.y *= UI_SCALE;
    return pos;
}
Vector2 _fPosFramebufferToScreen(Vector2 pos) {
    pos.x *= __state.window_scale;
    pos.y *= __state.window_scale;
    return pos;
}
Vector2 _fPosOverlayToScreen(Vector2 pos) {
    return _fPosFramebufferToScreen(_fPosOverlayToFramebuffer(pos));
}
Vector2 _fPosFramebufferToOverlay(Vector2 pos) {
    pos.x *= UI_SCALE;
    pos.y *= UI_SCALE;
    return pos;
}
Vector2 _fPosOverlayToFramebuffer(Vector2 pos) {
    pos.x /= UI_SCALE;
    pos.y /= UI_SCALE;
    return pos;
}

RLRectangle _fRectFramebufferToOverlay(RLRectangle rect) {
    rect.x *= UI_SCALE;
    rect.y *= UI_SCALE;
    rect.width *= UI_SCALE;
    rect.height *= UI_SCALE;
    return rect;
}
