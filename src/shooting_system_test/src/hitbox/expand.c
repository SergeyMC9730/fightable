#include <hitbox.h>
#include <math.h>

RLRectangle _fHitboxExpand(RLRectangle *h, float x, float y) {
    RLRectangle newh = {};

    if (x < 0.f) {
        newh.x += x;
        newh.width += fabs(x);
    }

    if (x > 0.f) {
        newh.width += x;
    }

    if (y < 0.f) {
        newh.y += y;
        newh.height += fabs(y);
    }

    if (y > 0.f) {
        newh.height += y;
    }

    return newh;
}