
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <fightable/entity.h>
#include <fightable/entity_library.h>

struct fentity *_fLevelFindPlayer(struct flevel *level) {
    if (!level || !level->entities || level->entities->len == 0) return 0;

    RSB_RDLOCK_BEGIN(level->entities);
    for (unsigned int i = 0; i < level->entities->len; i++) {
        struct fentity* entity = RSBGetAtIndex_fentity(level->entities, i);
        if (!entity) continue;

        if (entity->global_entity_id == ENTITY_PLAYER) {
            RSB_RDLOCK_END(level->entities);
            return entity;
        }
    }
    RSB_RDLOCK_END(level->entities);

    return 0;
}
