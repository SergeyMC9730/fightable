#define WITH_PLACEHOLDERS
#include <fightable/entity.h>

void _fEntityInit(struct fentity* entity) {
    entity->update = _fEntityUpdate;
    entity->draw = _fEntityDraw;
    entity->cleanup = _fEntityCleanup;

    return;
}