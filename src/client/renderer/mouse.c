#include <fightable/renderer.h>
#include <fightable/state.h>

Vector2 _fGetMousePosPix() {
    Vector2 mouse_pos = GetMousePosition();

    mouse_pos.x -= __state.mouse_pos_offset.x;
    mouse_pos.y -= __state.mouse_pos_offset.y;

    return _fPosScreenToFramebuffer(mouse_pos);
}
Vector2 _fGetMousePosOverlay() {
    return _fPosFramebufferToOverlay(_fGetMousePosPix());
}
