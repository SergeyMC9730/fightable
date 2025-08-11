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
