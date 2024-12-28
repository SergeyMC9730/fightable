#include <fightable/level.h>
#include <fightable/entity.h>
#include <fightable/entity_library.h>

struct fentity *_fLevelFindPlayer(struct flevel *level) {
    if (!level || !level->entities || level->entities->added_elements == 0) return 0;

    for (unsigned int i = 0; i < level->entities->added_elements; i++) {
        struct fentity* entity = RSBGetAtIndex_fentity(level->entities, i);
        if (!entity) continue;

        if (entity->global_entity_id == ENTITY_PLAYER) return entity;
    }

    return 0;
}