#include <fightable/hitbox.h>
#include <math.h>

fhitbox _fHitboxExpand(fhitbox *h, float x, float y) {
    fhitbox newh = {0};

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