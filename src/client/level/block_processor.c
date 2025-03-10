#include "fightable/time.h"
#include "raylib.h"
#include <fightable/level.h>
#include <fightable/block.h>
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
}
void _fLevelLoadProcessor(struct flevel *level) {
    TraceLog(LOG_INFO, "Loading level's tick thread");

    level->block_p_profile = 1;

    pthread_create(&level->block_processor_thread, NULL, _fLevelDoBlockUpdate, level);
}
