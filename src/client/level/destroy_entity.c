
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <fightable/entity.h>

void _fLevelDestroyEntity(struct flevel* level, struct fentity* entity) {
    if (!level || !entity || !level->entities) return;

    RSB_WRLOCK_BEGIN(level->entities);
    unsigned int idx = 0xFFFFFFFF;
    for (unsigned int i = 0; i < level->entities->len; i++) {
        if (level->entities->objects[i] == entity) {
            idx = i;
            break;
        }
    }

    if (idx == 0xFFFFFFFF) return;

    RSBPopElementAtIndex_fentity(level->entities, idx);
    RSB_WRLOCK_END(level->entities);
}
