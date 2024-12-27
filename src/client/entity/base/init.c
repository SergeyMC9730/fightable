#define WITH_PLACEHOLDERS
#include <fightable/entity.h>

void _fEntityInit(struct fentity* entity) {
    entity->update = _fEntityUpdate;
    entity->draw = _fEntityDraw;
    entity->cleanup = _fEntityCleanup;
    entity->max_hp = 20;
    entity->hp = entity->max_hp;
    entity->max_damage_colddown = 0.5f;
    entity->damage_colddown = 0;
    entity->tint = WHITE;

    return;
}