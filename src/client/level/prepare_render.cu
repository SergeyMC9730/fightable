#include <fightable/level.h>
#include <fightable/tilemap.h>
#include <fightable/block.h>
#include <fightable/state.h>
#include <chrono>
#include <cmath>

__device__ unsigned char rlCheckCollisionPointRec(Vector2 point, Rectangle rec) {
    if ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height))) {
        return 1;
    }

    return 0;
}

__global__ void _fLevelOptimizeLevelRenderingCuda(Rectangle area, fblock *blocks, unsigned char tile_w,unsigned char tile_h, int block_amount, unsigned char *allows) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i >= block_amount) return;

    fblock block = blocks[i];
    // check if block id is 0
    if (block.base.tile_x == 40 && block.base.tile_y == 7) return;

    allows[i] = rlCheckCollisionPointRec(Vector2{(float)block.base.block_x * tile_w, (float)block.base.block_y * tile_h}, area);
}

void _fLevelPrepareCudaRender(struct flevel *level, Rectangle area) {
    // cudaDeviceSynchronize();

    int n = level->data_size;
    static constexpr float entries = 1.f;

    auto start_time = std::chrono::high_resolution_clock::now();

    int blocks = (int)std::ceil((double)n / entries);

    _fLevelOptimizeLevelRenderingCuda<<<blocks, (int)entries>>>(area, level->dev_block_pipeline, level->tilemap->tile_size.x, level->tilemap->tile_size.y, n, level->dev_allow_pipeline);

    cudaMemcpy(level->host_allow_pipeline, level->dev_allow_pipeline, n, cudaMemcpyDeviceToHost);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    __state.cuda_time = (double)time.count() / (double)1000000;
}