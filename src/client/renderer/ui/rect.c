#include <fightable/rect.h>

void _fRectDraw(Rectangle r, Color grad_top, Color grad_bottom, Color container) {
    if (container.a != 0) {
        DrawRectangleRec(r, container);
    }

    DrawRectangleGradientV(r.x, r.y, 1, r.height, grad_top, grad_bottom);
    DrawRectangleGradientV(r.x + r.width, r.y, 1, r.height, grad_top, grad_bottom);

    DrawRectangle(r.x, r.y, r.width, 1, grad_top);
    DrawRectangle(r.x, r.y + r.height, r.width + 1, 1, grad_bottom);
}