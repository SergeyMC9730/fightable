#include <fightable/level.h>
#include <fightable/block.h>

void _fLevelDestroyPtx(struct flevel *level) {
    if (!level) return;

    if (level->dev_block_pipeline) cudaFree(level->dev_block_pipeline);
    if (level->dev_allow_pipeline) cudaFree(level->dev_allow_pipeline);

    level->dev_block_pipeline = NULL;
    level->dev_allow_pipeline = NULL;

    free(level->host_allow_pipeline);
    level->old_block_amount = 0;
}
