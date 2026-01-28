# Cuda

## environment
- 使用CMake引入，运行接口抽象成c语言类型
- 以.cu 后缀，有专门的 CUDA 编译器 编译 .cu 文件
    - 基本完全兼容 C++ 


## Abstraction

- 可定义由 cpu 调用的 gpu 运行函数，称为 kernel函数 ; 

- 可定义由 gpu 调用的 ，gpu运行的函数，被称为 device函数 ；
- 从cpu的视角，核函数是异步的。
    - 一定无返回值
- 定义cpu上运行的函数为 host 函数；
- 调用规则
    - host func -> kernel func -> device func 
    - device func <-> device func
    - kernel func <-> kernel func
- 支持区别于标准 C++ 的内联特性
- 支持 cuda_arch 宏定义，用于在不同 cuda 版本选择不同的代码运行

- 板块 block,一个block可由多个线程组成。
    - 一个 block 内的线程，共享相同的缓存
- 网格 grid，grid由多个block组成
- 调用 kernel 函数时们可以指定block数量，block内线程的数量
- kernel 函数内可访问特殊变量-线程编号，线程数量，block编号，block数量

- 错误处理，要自动收集gpu的运行状态
- 支持分配gpu内存，使用**指针** 操作内存
- 允许 跨 cpu/gpu 内存拷贝, 拷贝行为会强制cpu和gpu自动同步  
- 支持统一内存技术, 让驱动管理内存的拷贝，有开销
- 元编程
    - std::vector 第二模板参数 ，支持指定 cuda分配器
    - kernel 函数可以是模板函数
    - kernel 函数支持接受 functor （只支持按值传递）
    
## implememt
###  内存管理
- [内存搬运](./mem_demo.cu)

### 数组
- [多项式求值器](./poly_calc.cu)
> 在 GPU 上创建一个海量浮点数组（例如 100 万个元素），并对数组中的每个元素 $x$ 并行计算二次多项式 $y = ax^2 + bx + c$。

### C++ 封装
- [重力模拟]
> 利用 std::vector 管理 GPU 内存，结合模板和 Lambda 表达式，实现一个简单的粒子物理模拟（位置更新与速度变化）。目标是让 main 函数看起来完全像是在写普通的 C++ 代码，看不到 cudaMalloc 或 <<<>>> 的影子。


### TODO
https://aistudio.google.com/prompts/1npdWEt87-f2v25rXogkdTWZcwdWgh6hR