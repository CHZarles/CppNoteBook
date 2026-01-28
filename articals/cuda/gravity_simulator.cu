#include <stdio.h>
#include "common.h"
#include <vector>



template<typename T>
struct CudaAllocator {
    // value_type
    using value_type = T;
    // allocate
    T* allocate(std::size_t n) {
        T* ptr;
        /*
        这样分配
        出来的地址，不论在 CPU 还是 GPU 上都
        是一模一样的，都可以访问。而且拷贝也
        会自动按需进行（当从 CPU 访问时），无
        需手动调用 cudaMemcpy，大大方便了编
        程人员，特别是含有指针的一些数据结构
        */
        checkCudaErrors(cudaMallocManaged(&ptr, n * sizeof(T)));
        return ptr; 
    }

    // deallocate
    void deallocate(T* ptr, std::size_t n) {
        checkCudaErrors(cudaFree(ptr));
    }

    // construct
    template<class ...Args>
    void construct(T *p , Args&&... args) {
        // 优化： 只有当 T 不是 POD 类型，或者有参数传入时，才执行构造函数。
        if constexpr (!(sizeof...(Args) == 1 && std::is_pod_v<T>)) {
            ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
        }
    }
};




// 模板函数，接受一个可调用对象 Func
template<class Func>
__global__ void kernel_for(const int N, Func func){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int stride = blockDim.x * gridDim.x;
    for(int i = idx; i < N; i += stride){
        func(i);
    }
}

// 实现一个可调用对象
struct UpdateVelocity{
  
    const float A = 9.8f; 
    const float deltaTime = 0.01f; 
    float* vel;    // 不要放 std::vector

    UpdateVelocity(float* velocities) : vel(velocities) {}

   // kernel 可调用的函数
    __device__ void operator()(int i) {
        vel[i] += A * deltaTime;
      
    }
};

template <typename Func>
void parallel_for(int n, Func f) {
    // 1.配置 Block 大小 
    const int blockSize = 256;

    // 2. 配置 Grid 大小
    int gridSize = (n + blockSize - 1) / blockSize;

    if(gridSize > 2048){
        gridSize = 2048; // 限制最大 Grid 大小，避免启动过多的线程块
    }

    kernel_for<<<gridSize, blockSize>>>(n, f);
    

    checkCudaErrors(cudaGetLastError()); // 捕获启动时的参数错误
    checkCudaErrors(cudaDeviceSynchronize()); // 捕获执行时的错误并等待完成
}
constexpr int N = 1000;
int main(){

    std::vector<float, CudaAllocator<float>> velocities(N);
    parallel_for(N, UpdateVelocity(velocities.data()));

    for(int i = 0; i < 1000; ++i){
        printf("Velocity[%d] = %f\n", i, velocities[i]);    
    }
}