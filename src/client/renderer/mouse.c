#include <fightable/renderer.h>
#include <fightable/state.h>

Vector2 _fGetMousePosPix() {
    Vector2 mouse_pos = GetMousePosition();

    mouse_pos.x -= __state.mouse_pos_offset.x; mouse_pos.x /= __state.window_scale;
    mouse_pos.y -= __state.mouse_pos_offset.y; mouse_pos.y /= __state.window_scale;

    return mouse_pos;
}
Vector2 _fGetMousePosOverlay() {
    Vector2 mouse_pos = _fGetMousePosPix();
    mouse_pos.x *= UI_SCALE; mouse_pos.y *= UI_SCALE;

    return mouse_pos;
}