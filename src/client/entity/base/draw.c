#include <fightable/entity.h>

void _fEntityDraw(struct fentity *entity) {
    if (!entity) return;

    fhitbox hitbox = entity->hitbox;
    hitbox.y -= 2;

    DrawRectangleRec(hitbox, BLUE);
}