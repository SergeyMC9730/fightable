#include <fightable/level.h>
#include <fightable/entity.h>

void _fLevelDestroyEntity(struct flevel* level, struct fentity* entity) {
    if (!level || !entity || !level->entities) return;

    unsigned int idx = 0xFFFFFFFF;
    for (unsigned int i = 0; i < level->entities->added_elements; i++) {
        if (level->entities->objects[i] == entity) {
            idx = i;
            break;
        }
    }

    if (idx == 0xFFFFFFFF) return;

    RSBPopElementAtIndex_fentity(level->entities, idx);
}
