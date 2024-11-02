#include <fightable/hitbox.h>

void _fHitboxMove(fhitbox *h1, float x, float y) {
    h1->x += x;
    h1->y += y;
}