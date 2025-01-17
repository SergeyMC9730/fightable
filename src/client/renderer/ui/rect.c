#include <fightable/rect.h>

void _fRectDraw(RLRectangle r, Color grad_top, Color grad_bottom, Color container) {
    if (container.a != 0) {
        DrawRectangleRec(r, container);
    }

    DrawRectangleGradientV(r.x, r.y, 1, r.height, grad_top, grad_bottom);
    DrawRectangleGradientV(r.x + r.width, r.y, 1, r.height, grad_top, grad_bottom);

    DrawRectangle(r.x, r.y, r.width, 1, grad_top);
    DrawRectangle(r.x, r.y + r.height, r.width + 1, 1, grad_bottom);
}

RLRectangle _fRectAbsSize(RLRectangle r) {
    if (r.width < 0) {
        r.x += r.width;
        r.width = -r.width;
    }
    if (r.height < 0) {
        r.y += r.height;
        r.height = -r.height;
    }

    return r;
}
