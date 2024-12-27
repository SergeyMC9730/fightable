#define WITH_PLACEHOLDERS
#include <fightable/player.h>

void _flPlayerUpdate(struct felplayer* instance) {
    struct fentity* entity = &instance->base;

    entity->moving_horizontally = 0;
    entity->moving_negative = 0;

    if (!entity->dead) {
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
            entity->moving_horizontally = 1;
            entity->moving_negative = 0;
        }
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
            entity->moving_horizontally = 1;
            entity->moving_negative = 1;
        }
        if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE))) {
            _fEntityJump(entity, 1);
        }
        if ((IsKeyPressed(KEY_W) || IsKeyPressed(KEY_SPACE))) {
            _fEntityJump(entity, 0);
        }
    }

    _fEntityUpdate(entity);
}