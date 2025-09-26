
//          Sergei Baigerov 2024 - 2025.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS

#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/time.h>
#include <time.h>

void *_fLevelDoBlockUpdate(void* _level) {
    struct flevel *level = (struct flevel *)_level;
    if (!level) return NULL;

    clock_t start;
    clock_t end;

    while (!level->block_p_close) {
        if (!level->pause_world) {
            start = clock();
            _fLevelTick(level);
            end = clock();
            double t = ((double)(end - start) / (double)CLOCKS_PER_SEC) * 1000.f;
            int req = (int)((1000.f / level->tps) - t);
            if (level->tps > 0.f) _fSleep(req);
            if (level->block_p_profile) {
                TraceLog(LOG_INFO, "Level ticked in %fms; Wait %d ms", (float)t, req);
            }
        } else {
            if (level->tps > 0.f) _fSleep((int)(1000.f / level->tps));
        }
    }

    TraceLog(LOG_INFO, "Closing tick thread");

    return NULL;
}
void _fLevelTick(struct flevel* level) {
    if (!level) return;

    for (unsigned int i = 0; i < level->data_size; i++) {
        _fBlockUpdate(level->objects + i, level);
    }

    struct fentity *player = 0;

    float delta = 1.f / level->tps;

    if (level->entities) {
        player = _fLevelFindPlayer(level);
    }

    if (level->entities && level->hitboxes) {
        for (int i = 0; i < level->entities->len; i++) {
            struct fentity* entity = RSBGetAtIndex_fentity(level->entities, i);
            if (!entity || entity == player) continue;

            entity->obstacles = level->hitboxes;
            entity->obstacles_length = level->data_size;
            entity->custom_delta = delta;
            entity->update(entity);
        }
    }
}
void _fLevelLoadProcessor(struct flevel *level) {
    if (level->block_p_loaded) return;

    TraceLog(LOG_INFO, "Loading level's tick thread");

    level->block_p_profile = 0;
    level->block_p_loaded = 1;

    pthread_create(&level->block_processor_thread, NULL, _fLevelDoBlockUpdate, level);
}
