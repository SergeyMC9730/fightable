#include <fightable/hitbox.h>

unsigned char _fHitboxIntersects(fhitbox *h1, fhitbox *h2) {
    if (h2->x + h2->width <= h1->x || h2->x >= h1->x + h1->width) {
        return 0;
    }
    if (h2->y + h2->height <= h1->y || h2->y >= h1->y + h1->height) {
        return 0;
    }

    return 1;
}