#include <fightable/level.h>
#include <fightable/block.h>

void _fLevelReloadCudaCtx(struct flevel *level) {
    if (level->old_block_amount != level->data_size) {
        TraceLog(LOG_INFO, "Updating CUDA context (old size: %d, new size: %d)", level->old_block_amount, level->data_size);

        if (level->host_allow_pipeline) {
            level->host_allow_pipeline = (unsigned char *)realloc(level->host_allow_pipeline, level->data_size);
        } else {
            level->host_allow_pipeline = (unsigned char *)malloc(level->data_size);
        }
        

        if (level->dev_block_pipeline) {
            cudaFree(level->dev_block_pipeline);
        }
        cudaMalloc(&level->dev_block_pipeline, sizeof(fblock) * level->data_size);
        cudaMemcpy(level->dev_block_pipeline, level->objects, sizeof(fblock) * level->data_size, cudaMemcpyHostToDevice);

        if (level->dev_allow_pipeline) {
            cudaFree(level->dev_allow_pipeline);
        }
        cudaMalloc(&level->dev_allow_pipeline, level->data_size);
        
        level->old_block_amount = level->data_size;

        TraceLog(LOG_INFO, "Update complete");
    }
}