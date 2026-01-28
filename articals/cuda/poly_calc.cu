#include <stdio.h>
#include "common.h"
#include <vector>


constexpr int N = 1000003;
using ElementType = float;
constexpr ElementType a = 2.5f, b = 1.0f, c = 5.0f;

/*

/*

graph of thread , grid, block in CUDA

      +-----------------------------------------------------------------------+
      |                                 GRID                                  |
      |                      (Kernel Launch / Host View)                      |
      |                                                                       |
      |  +-----------------------+             +-----------------------+      |
      |  |      BLOCK (0, 0)     |             |      BLOCK (1, 0)     |      |
      |  |   blockIdx.x = 0      |             |   blockIdx.x = 1      |      |
      |  |   blockIdx.y = 0      |             |   blockIdx.y = 0      |      |
      |  +-----------+-----------+             +-----------------------+      |
      |              |                                                        |
      |              v                                                        |
      |      (Zoom in to Block)                                               |
      |                                                                       |
      |  +-------------------------------------------------------------+      |
      |  |                       BLOCK (0, 0)                          |      |
      |  |              Executed on Streaming Multiprocessor (SM)      |      |
      |  +-------------------------------------------------------------+      |
      |  |                       SHARED MEMORY                         |      |
      |  |              (Visible to all threads in this block)         |      |
      |  +-------------------------------------------------------------+      |
      |  |                                                             |      |
      |  |  +-------------------+       +-------------------+          |      |
      |  |  |    THREAD (0,0)   |       |    THREAD (1,0)   |          |      |
      |  |  |  threadIdx.x = 0  |       |  threadIdx.x = 1  |   ...    |      |
      |  |  |  threadIdx.y = 0  |       |  threadIdx.y = 0  |          |      |
      |  |  | +---------------+ |       | +---------------+ |          |      |
      |  |  | |   Registers   | |       | |   Registers   | |          |      |
      |  |  | +---------------+ |       | +---------------+ |          |      |
      |  |  | |  Local Memory | |       | |  Local Memory | |          |      |
      |  |  | +---------------+ |       | +---------------+ |          |      |
      |  |  +-------------------+       +-------------------+          |      |
      |  |                                                             |      |
      |  |  +-------------------+       +-------------------+          |      |
      |  |  |    THREAD (0,1)   |       |    THREAD (1,1)   |          |      |
      |  |  |  threadIdx.x = 0  |       |  threadIdx.x = 1  |   ...    |      |
      |  |  |  threadIdx.y = 1  |       |  threadIdx.y = 1  |          |      |
      |  |  | +---------------+ |       | +---------------+ |          |      |
      |  |  | |   Registers   | |       | |   Registers   | |          |      |
      |  |  | +---------------+ |       | +---------------+ |          |      |
      |  |  +-------------------+       +-------------------+          |      |
      |  |                                                             |      |
      |  +-------------------------------------------------------------+      |
      |                                                                       |
      |                                                                       |
      |  +-----------------------+             +-----------------------+      |
      |  |      BLOCK (0, 1)     |     ...     |      BLOCK (1, 1)     |      |
      |  +-----------------------+             +-----------------------+      |
      |                                                                       |
      +-----------------------------------------------------------------------+
      |                            GLOBAL MEMORY                              |
      |                (Visible to all Blocks and Threads)                    |
      +-----------------------------------------------------------------------+

*/


__global__ void calc(ElementType* X, ElementType* Y ){

    // 网格跨步循环 (The Grid-Stride Loop) , 适应不同规模的数据计算
    // 1. 起始点：全局线程索引 (Global Thread ID)
    //    不仅要看自己在班级(Block)里的座位号(threadIdx)，还要看班级号(blockIdx)
    int st = blockIdx.x * blockDim.x + threadIdx.x;

    // 2. 步长：网格总宽度 (Grid Stride)
    //    总共有多少个线程在跑？ = 班级数量 * 班级人数
    int stride = gridDim.x * blockDim.x;
    for(int i = st ; i < N; i += stride){
        Y[i] = a * X[i] * X[i] + b * X[i] + c;
    }
}



int main(){
    // 分配量数组
    ElementType *X,*Y;
    exitIfError(cudaMalloc(&X, N * sizeof(ElementType)));
    exitIfError(cudaMalloc(&Y, N * sizeof(ElementType)));

    // 初始化X 0.0 1.0 2.0 ... N-1.0
    std::vector<ElementType> h_X(N);
    for(int i = 0; i < N; ++i){
        h_X[i] = static_cast<ElementType>(i);
    }
    exitIfError(cudaMemcpy(X, h_X.data(), N * sizeof(ElementType), cudaMemcpyHostToDevice));

    // 运算
    int block_size = 256;
    int grid_size = 32 ; // (N + block_size - 1) / block_size;
    calc<<<grid_size, block_size>>>(X,Y);


    // 拷贝结果回host
    std::vector<ElementType> h_Y(N);
    exitIfError(cudaMemcpy(h_Y.data(), Y, N * sizeof(ElementType), cudaMemcpyDeviceToHost));

    // 对比第N-1个元素
    int idx = N - 1;
    ElementType cpu_result = a * h_X[idx] * h_X[idx] + b * h_X[idx] + c;
    printf("GPU result: %f, CPU result: %f\n", h_Y[idx], cpu_result);

    // 释放内存
    cudaFree(X);
    cudaFree(Y);

}