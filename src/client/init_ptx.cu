#include <fightable/renderer.h>
#include <fightable/state.h>

void _fPtxInit() {
    TraceLog(LOG_INFO, "Initializing CUDA");

    cudaError_t status = cudaInitDevice(0, 0, 0);
    if (status != cudaSuccess) {
        TraceLog(LOG_ERROR, "Could not initialize CUDA: %d", (int)status);
    } else {
        __state.can_use_gpu_accel = 1;
    }
}