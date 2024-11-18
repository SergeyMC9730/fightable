#include <stdio.h>
#include <chrono>

// Vector2, 2 components
typedef struct Vector2 {
    float x;                // Vector x component
    float y;                // Vector y component
} Vector2;

// Rectangle, 4 components
typedef struct Rectangle {
    float x;                // Rectangle top-left corner position x
    float y;                // Rectangle top-left corner position y
    float width;            // Rectangle width
    float height;           // Rectangle height
} Rectangle;

__device__ unsigned char CheckCollisionPointRec(Vector2 point, Rectangle rec) {
    if ((point.x >= rec.x) && (point.x < (rec.x + rec.width)) && (point.y >= rec.y) && (point.y < (rec.y + rec.height))) {
        return 1;
    }

    return 0;
}

__global__ void helloFromGPU(Rectangle area, Vector2 *blocks, unsigned char tile_w,unsigned char tile_h, int block_amount, unsigned char *allows) {
    int i = blockDim.x * blockIdx.x + threadIdx.x;
    if (i >= block_amount) return;

    Vector2 block = blocks[i];
    allows[i] = CheckCollisionPointRec(Vector2{block.x * tile_w, block.y * tile_h}, area);
}

#include "test.h"

void setup() {
    cudaInitDevice(0, 0, 0);
}

void performGpuAction() {
    constexpr int n = 128;
    Vector2 *host_objects = (Vector2 *)calloc(128, sizeof(Vector2));
    unsigned char *host_allows = (unsigned char *)calloc(128, 1);

    for (int i = 0; i < n; i++) {
        Vector2 *ref = host_objects + i;
        ref->x = i;
        ref->y = i;
    }

    Vector2 *gpu_objects;
    cudaMalloc(&gpu_objects, sizeof(Vector2) * n);

    unsigned char *gpu_allows;
    cudaMalloc(&gpu_allows, 1 * n);

    Rectangle area = (Rectangle){
        0,
        0,
        160,
        120
    };

    auto start_time = std::chrono::high_resolution_clock::now();

    cudaMemcpy(gpu_objects, host_objects, 128 * sizeof(Vector2), cudaMemcpyHostToDevice);

    constexpr int threadsPerBlock = 128;
    constexpr int blocksPerGrid = (n + threadsPerBlock - 1) / threadsPerBlock;
    helloFromGPU<<<blocksPerGrid, threadsPerBlock>>>(area, gpu_objects, 8, 8, n, gpu_allows);

    cudaMemcpy(host_allows, gpu_allows, n, cudaMemcpyDeviceToHost);

    auto end_time = std::chrono::high_resolution_clock::now();
    auto time = end_time - start_time;

    for (int i = 0; i < n; i++) {
        printf("obj %f:%f -> %d\n", host_objects[i].x, host_objects[i].x, host_allows[i]);
    }

    free(host_objects);
    free(host_allows);

    cudaFree(gpu_allows);
    cudaFree(gpu_objects);

    cudaDeviceReset();

    printf("\n-----------\ntook %ld ns to execute\n-----------\n",
        time.count()
    );
}