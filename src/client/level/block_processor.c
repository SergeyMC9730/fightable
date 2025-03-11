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
        if (level->block_p_profile) start = clock();
        _fLevelTick(level);
        if (level->block_p_profile) {
            end = clock();
            double t = ((double)(end - start) / (double)CLOCKS_PER_SEC) * 1000.f;

            TraceLog(LOG_INFO, "Level ticked in %fms", (float)t);
        }

        if (level->tps > 0.f) _fSleep((int)(1000.f / level->tps));
    }

    TraceLog(LOG_INFO, "Closing tick thread");

    return NULL;
}
void _fLevelTick(struct flevel* level) {
    if (!level) return;

    for (unsigned int i = 0; i < level->data_size; i++) {
        _fBlockUpdate(level->objects + i, level);
    }

    RLRectangle *rects = 0;
    struct fentity *player = 0;

    float delta = 1.f / level->tps;

    if (level->entities) {
        rects = _fLevelGetHitboxes(level);
        player = _fLevelFindPlayer(level);
    }

    if (level->entities && rects) {
        for (int i = 0; i < level->entities->added_elements; i++) {
            struct fentity* entity = RSBGetAtIndex_fentity(level->entities, i);
            if (!entity || entity == player) continue;

            entity->obstacles = rects;
            entity->obstacles_length = level->data_size;
            entity->custom_delta = delta;

            if (!entity->update) {
                _fEntityUpdate(entity);
            }
            else {
                entity->update(entity);
            }
        }
    }

    if (rects) MemFree(rects);
}
void _fLevelLoadProcessor(struct flevel *level) {
    TraceLog(LOG_INFO, "Loading level's tick thread");

    level->block_p_profile = 0;

    pthread_create(&level->block_processor_thread, NULL, _fLevelDoBlockUpdate, level);
}
