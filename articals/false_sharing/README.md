# 伪共享与 Cache Line

## 现象

两个线程分别递增两个原子计数器。即使两个线程没有访问同一个变量，相邻布局仍可能比填充布局慢：

```text
AdjacentCounters:  [ first ][ second ]       （通常在同一条 64 字节 Cache line）
PaddedCounters:    [ first ............... ] [ second .............. ]
```

CPU 以 Cache line 为单位维护缓存一致性。相邻布局中，一个核心写入 `first` 会让其他核心缓存的整条 Cache line 失效；另一个核心写入 `second` 时又会触发相同过程。两个线程因此反复争用同一条 Cache line，这种现象叫**伪共享**（false sharing）。

填充布局让两个热点字段从不同的 Cache line 开始，减少缓存一致性流量。这里使用 `alignas(64)`，并通过字段偏移的 `static_assert` 检查布局。

## 代码

- [`include/false_sharing.h`](./include/false_sharing.h)：相邻布局和填充布局
- [`src/false_sharing_demo.cpp`](./src/false_sharing_demo.cpp)：测量两种布局的耗时
- [`tests/test_false_sharing.cpp`](./tests/test_false_sharing.cpp)：检查布局和并发写入结果

示例使用 `memory_order_relaxed`，因为这里只关心每个原子计数器自身的并发递增，不需要用计数器建立线程间的先后关系。`relaxed` 保证原子性，但不会消除 Cache line 的一致性开销。

## 运行

```bash
cd articals/false_sharing
./run.sh
```

也可以直接使用 CMake：

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
./build/false_sharing_demo
```

单次耗时会受到线程调度、CPU 频率和系统负载影响。重点观察字段偏移满足布局约束，以及相邻布局在当前机器上通常更慢；不要把一次运行的耗时比当成固定常数。示例按常见的 64 字节 Cache line 编写，具体硬件应以目标平台为准。
