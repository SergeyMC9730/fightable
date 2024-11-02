#define WITH_PLACEHOLDERS
#include <fightable/player.h>

void _flPlayerUpdate(struct felplayer* instance) {
    struct fentity* entity = instance;

    entity->moving_horizontally = 0;
    entity->moving_negative = 0;

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        entity->moving_horizontally = 1;
        entity->moving_negative = 0;
    }
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        entity->moving_horizontally = 1;
        entity->moving_negative = 1;
    }
    if ((IsKeyDown(KEY_W) || IsKeyDown(KEY_SPACE))) {
        _fEntityJump(entity);
    }

    _fEntityUpdate(entity);
}