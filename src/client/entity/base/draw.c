#include <fightable/entity.h>

void _fEntityDraw(struct fentity *entity) {
    if (!entity) return;

    fhitbox hitbox = entity->hitbox;

    if ((entity->standing_object.width * entity->standing_object.height) > 0.f) {
        hitbox.y = entity->standing_object.y - entity->hitbox.height;
    }


    DrawRectangle((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, BLUE);
}