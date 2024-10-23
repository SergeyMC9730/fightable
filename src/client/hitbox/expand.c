#include <fightable/hitbox.h>
#include <math.h>

Rectangle _fHitboxExpand(Rectangle *h, float x, float y) {
    Rectangle newh = {};

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