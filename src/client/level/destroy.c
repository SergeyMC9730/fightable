
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#include <fightable/level.h>
#include <pthread.h>

void _fLevelDestroy(struct flevel* level, unsigned char level_allocated, unsigned char blocks_allocated, unsigned char entities_allocated) {
    if (!level) return;

    level->block_p_close = true;
    if (level->block_processor_thread) pthread_join(level->block_processor_thread, NULL);

    if (level->objects && blocks_allocated) free(level->objects);
    if (level->entities && entities_allocated) RSBDestroy_fentity(level->entities);
    if (level->block_entries) RSBDestroy_lre(level->block_entries);
    if (level->light_sources) RSBDestroy_lls(level->light_sources);

    level->objects = NULL;
    level->data_size = 0;
    level->entities = NULL;
    level->block_entries = NULL;
    level->light_sources = NULL;

    UnloadTexture(level->background_tile);

#ifdef COTARGET_PTX
    _fLevelDestroyPtx(level);
#endif

    if (level_allocated) free(level);
}
