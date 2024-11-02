#include <fightable/entity.h>

void _fEntityJump(struct fentity* entity) {
    if (entity->on_ground) {
        entity->speed.y = -32.5f;
    }
}