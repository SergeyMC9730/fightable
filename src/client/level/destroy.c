
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <pthread.h>
#include <fightable/pthread_compat.h>

void _fLevelDestroy(struct flevel* level, unsigned char level_allocated, unsigned char blocks_allocated, unsigned char entities_allocated) {
    if (!level) return;

    _fLevelUnloadProcessor(level);

    if (level->objects && blocks_allocated) free(level->objects);
    // TODO: FIX MEMORY LEAK
    if (level->entities && entities_allocated) RSBDestroy_fentity(level->entities);
    if (level->block_entries) RSBDestroy_lre(level->block_entries);
    if (level->light_sources) RSBDestroy_lls(level->light_sources);

    level->objects = NULL;
    level->data_size = 0;
    level->entities = NULL;
    level->block_entries = NULL;
    level->light_sources = NULL;

    UnloadTexture(level->background_tile);

    if (level_allocated) free(level);
}
