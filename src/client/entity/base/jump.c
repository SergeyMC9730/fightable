#include <fightable/entity.h>

void _fEntityJump(struct fentity* entity, unsigned char holding) {
    if (entity->on_ground) {
        entity->speed.y = -32.5f;
        entity->can_double_jump = 1;
    }
    else {
        if (holding) return;

        if (entity->can_double_jump) {
            entity->speed.y = -32.5f;
            entity->can_double_jump = 0;
        }
    }
}