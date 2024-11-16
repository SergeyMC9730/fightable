#include <fightable/level.h>
#include <fightable/tilemap.h>
#include <chrono>

__device__ unsigned char rlCheckCollisionPointRec(Vector2 point, Rectangle rec) {
    if ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height))) {
        return 1;
    }

    return 0;
}

__global__ void _fLevelOptimizeLevelRenderingCuda(Rectangle area, Vector2 *blocks, unsigned char tile_w,unsigned char tile_h, int block_amount, unsigned char *allows) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i >= block_amount) return;

    Vector2 block = blocks[i];
    allows[i] = rlCheckCollisionPointRec(Vector2{block.x * tile_w, block.y * tile_h}, area);
}

void _fLevelPrepareCudaRender(struct flevel *level, Rectangle area) {
    int n = level->data_size;

    // auto start_time = std::chrono::high_resolution_clock::now();

    constexpr int threadsPerBlock = 128;
    int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    _fLevelOptimizeLevelRenderingCuda<<<blocksPerGrid, threadsPerBlock>>>(area, level->dev_block_pipeline, level->tilemap->tile_size.x, level->tilemap->tile_size.y, n, level->dev_allow_pipeline);

    cudaMemcpy(level->host_allow_pipeline, level->dev_allow_pipeline, n, cudaMemcpyDeviceToHost);

    // auto end_time = std::chrono::high_resolution_clock::now();
    // auto time = end_time - start_time;

    // double time_ms = (double)time.count() / (double)1000000;
}