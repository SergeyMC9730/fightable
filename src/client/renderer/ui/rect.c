#include <fightable/rect.h>

void _fRectDraw(Rectangle r, Color grad_top, Color grad_bottom, Color container) {
    DrawRectangleRec(r, container);

    DrawRectangleGradientV(r.x, r.y, 1, r.height, grad_top, grad_bottom);
    DrawRectangleGradientV(r.x + r.width, r.y, 1, r.height, grad_top, grad_bottom);

    DrawLine(r.x, r.y, r.x + r.width, r.y, grad_top);
    DrawLine(r.x, r.y + r.height, r.x + r.width + 1, r.y + r.height, grad_bottom);
}