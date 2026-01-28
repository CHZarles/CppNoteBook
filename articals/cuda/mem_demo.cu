#include "common.h"
#include <vector>

constexpr int N = 100;   
__global__ void encryptKernel(int key, int *arr){
    for(int i = 0 ; i < N ; ++ i){
        arr[i] += key;
    }
}

int main(){
    // Allocate vector on host
    std::vector<int> vec(N, 0);
    int *h_vec = vec.data();
    // Allocate vector on gpu
    int *d_vec;
    exitIfError(cudaMalloc(&d_vec, N * sizeof(int)));
    cudaMemset(d_vec, 0, N * sizeof(int));

    // call kernel func
    encryptKernel<<<1,1>>>(1, d_vec);
    // D2H
    exitIfError(cudaMemcpy(h_vec, d_vec, sizeof(int) * N , cudaMemcpyDeviceToHost));

    // Show result
    for(int i = 0 ; i < N ; ++ i){
        printf("%d ", h_vec[i]);
    }
    printf("\n");

    // Free GPU memory
    cudaFree(d_vec);
    return 0;
}