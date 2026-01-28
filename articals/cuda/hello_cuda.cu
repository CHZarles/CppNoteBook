#include "common.h"

__global__ void hello_from_gpu()
{
    printf("Hello World from GPU!\n");
}

int main()
{
    // Launch kernel with 1 block of 10 threads
    hello_from_gpu<<<1, 10>>>();

    // Wait for GPU to finish before accessing on host
    exitIfError(cudaDeviceSynchronize());

    printf("Hello World from CPU!\n");
    return 0;
}