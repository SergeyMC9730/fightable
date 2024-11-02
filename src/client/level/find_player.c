#include <fightable/level.h>
#include <fightable/entity.h>

struct fentity *_fLevelFindPlayer(struct flevel *level) {
    if (!level || !level->entities || level->entity_data_size == 0) return 0;

    for (unsigned int i = 0; i < level->entity_data_size; i++) {
        struct fentity *entity = level->entities[i];
        if (!entity) continue;

        if (entity->global_entity_id == 1) return entity;
    }

    return 0;
}