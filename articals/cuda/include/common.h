#include <stdio.h>
#include  <cuda_runtime.h>
#include "helper_cuda.h"


// exit if get error
void exitIfError(cudaError_t status) {
    checkCudaErrors(status);
    if (status != cudaSuccess) {
        exit(EXIT_FAILURE);
    }
}