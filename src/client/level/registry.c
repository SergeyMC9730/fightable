
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <fightable/block.h>

struct flevel_registry_entry *_fLevelFindBlockEntry(struct flevel *level, unsigned int unique_block_id) {
    if (!level) return NULL;

    for (unsigned int i = 0; i < level->block_entries->len; i++) {
        struct flevel_registry_entry *e = level->block_entries->objects + i;
        if (e->id == unique_block_id) return e;
    }

    return NULL;
}

#include <stdio.h>

struct fblock *_fLevelBlockFromRegistry(struct flevel *level, unsigned int registry_id) {
    if (!level) return NULL;

    for (unsigned int i = 0; i < level->data_size; i++) {
        // printf("%d: %d\n", i, level->objects[i].registry_id);
        if (level->objects[i].registry_id == registry_id) {
            return level->objects + i;
        }
    }

    return NULL;
}
