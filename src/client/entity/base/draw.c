#include <fightable/entity.h>
#include <fightable/color.h>

void _fEntityDraw(struct fentity *entity) {
    if (!entity || entity->dead || entity->object_destroyed) return;

    fhitbox hitbox = entity->hitbox;

    if ((entity->standing_object.width * entity->standing_object.height) > 0.f) {
        hitbox.y = entity->standing_object.y - entity->hitbox.height;
    }

    Color col = BLUE;
    if (entity->damage_colddown != 0.f) {
        _fMixColors(BLUE, entity->tint, entity->damage_colddown * (1 / entity->max_damage_colddown));
    }
    DrawRectangle((int)hitbox.x, (int)hitbox.y, (int)hitbox.width, (int)hitbox.height, col);

    _fEntityDrawAccessory(entity);
}