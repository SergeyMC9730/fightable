#include <fightable/entity.h>

IVector2 _fEntityGetDrawingPos(struct fentity* entity) {
    if (!entity || entity->object_destroyed) return (IVector2) { 0 };

    fhitbox hitbox = entity->hitbox;

    if ((entity->standing_object.width * entity->standing_object.height) > 0.f) {
        hitbox.y = entity->standing_object.y - entity->hitbox.height;
    }

    return (IVector2) { hitbox.x, hitbox.y };
}