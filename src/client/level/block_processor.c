
//          Sergei Baigerov 2024 - 2026.
// Distributed under the Boost Software License, Version 1.0.
//    (See accompanying file LICENSE.txt or copy at
//          https://www.boost.org/LICENSE_1_0.txt)

#define WITH_PLACEHOLDERS

#include <fightable/level.h>
#include <fightable/block.h>
#include <fightable/entity.h>
#include <fightable/time.h>
#include <fightable/pthread_compat.h>
#include <fightable/state.h>
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
            __state.world_tick_time = t;
            // __state.world_tick_time_ext = t + ((float)req / 1000.f);
        } else {
            if (level->tps > 0.f) _fSleep((int)(1000.f / level->tps));
        }
    }

    TraceLog(LOG_INFO, "Closing tick thread");

    return NULL;
}
void _fLevelTick(struct flevel* level) {
    if (!level) return;

    // for (unsigned int i = 0; i < level->data_size; i++) {
    //     _fBlockUpdate(level->objects + i, level);
    // }

    struct fentity *player = 0;

    float delta = 1.f / level->tps;

    if (level->entities) {
        player = _fLevelFindPlayer(level);
    }

    if (level->entities && level->hitboxes) {
        // TraceLog(LOG_INFO, "lock=%d", level->entities->lock);
        unsigned int len = level->entities->len;
        for (int i = 0; level->entities && i < len; i++) {
            RSB_RDLOCK(level->entities, struct fentity* entity = RSBGetAtIndex_fentity(level->entities, i););
            if (!entity) continue;
            if (entity->dead) continue;
            if (entity->hitbox.hitbox.y > 1024) {
                entity->damage(entity, 4);
            }
            if (entity == player) continue;

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

void _fLevelUnloadProcessor(struct flevel *level) {
    if (!level->block_p_loaded) return;

    TraceLog(LOG_INFO, "Unloading level's tick thread");

    level->block_p_close = 1;
    if (!_fComparePthreadAndEmptyThread(level->block_processor_thread)) {
        pthread_join(level->block_processor_thread, NULL);
    }
    level->block_p_loaded = 0;
    level->block_p_close = 0;
}
