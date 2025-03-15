# TBB 并行编程

## 一个基本程序

```c++
#include <iostream>
#include <string>
#include <tbb/task_group.h>
#include <thread>

void download(std::string file) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%) ... "
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << "\n";
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}
int main() {
  tbb::task_group tg;
  tg.run([&] { download("hello.zip"); });
  tg.run([&] { interact(); });
  tg.wait();
  return 0;
}

```

- 用一个任务组 tbb::task_group 启动多个任务，一个负责下载，一个负责和用户交互。并在主线程中等待该任务组里的任务全部执行完毕。

- 和多线程的用法相比，一个任务不一定对应一个线程，如果任务数量超过CPU最大的线程数，会由 TBB 在用户层负责调度任务运行在多个预先分配好的线程，而不是由操作系统负责调度线程运行在多个物理核心。

## 封装好了：parallel_invoke

```c++
#include <iostream>
#include <string>
#include <tbb/parallel_invoke.h>

int main() {
  std::string s = "hello, world!";
  char ch = 'l';
  tbb::parallel_invoke(
      [&] {
        for (int i = 0; i < s.size() / 2; ++i) {
          if (s[i] == ch)
            std::cout << "Found char " << ch << " at position " << i
                      << std::endl;
        }
      },
      [&] {
        for (int i = s.size() / 2; i < s.size(); ++i) {
          if (s[i] == ch)
            std::cout << "Found char " << ch << " at position " << i
                      << std::endl;
        }
      });
  return 0;
}
```

## 并行循环

```c++
void case0() {
  size_t n = 1 << 26;
  std::cout << "n = " << n << std::endl;
  std::vector<float> a(n);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
                    [&](const tbb::blocked_range<size_t> &r) {
                      for (size_t i = r.begin(); i != r.end(); ++i) {
                        a[i] = std::sqrt(a[i]);
                      }
                    });
}
```

1. 定义数组的大小 `n`，这里 `n` 是 \(2^{26}\)：

   ```cpp
   size_t n = 1 << 26;
   ```

2. 创建一个大小为 `n` 的浮点数向量 `a`：

   ```cpp
   std::vector<float> a(n);
   ```

3. 使用 `tbb::parallel_for` 并行处理数组 `a`：

   ```cpp
   tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
                     [&](const tbb::blocked_range<size_t> &r) {
                       for (size_t i = r.begin(); i != r.end(); ++i) {
                         a[i] = std::sqrt(a[i]);
                       }
                     });
   ```

   - `tbb::blocked_range<size_t>(0, n)` 定义了并行处理的范围，从 0 到 `n`。
   - Lambda 函数 `&[&](const tbb::blocked_range<size_t> &r)` 定义了并行执行的操作。
   - 在每个并行块中，计算数组 `a` 中每个元素的平方根。

### 其他用法

```c++
// 基于下标
void parallel_for(Index first , Index last , const Function &f);
// 基于迭代器区间
void parallel_for_each(Iterator first, Iterator last, const Body &body);
```

详细调用例子 [parallel_for.cpp](./parallel_for.cpp)

## 缩并 (reduce)

在并行计算中，reduce（归约）是一种常见的操作，用于将多个值组合成一个单一的值。这个过程通常涉及对一组数据应用一个二元操作（如加法、乘法、最大值等），并将结果逐步合并。

例如，在并行计算中，可以将一个大数组分成多个子数组，并行地对每个子数组进行归约操作，然后再将这些中间结果合并成最终结果。

这种技术在处理大规模数据时非常有用，因为它可以显著减少计算时间。常见的并行归约操作包括求和、求积、求最大值和最小值等。

```bash
大数组: [1, 2, 3, 4, 5, 6, 7, 8]

分割成子数组:
[1, 2] [3, 4] [5, 6] [7, 8]

并行归约操作:
  1 + 2 = 3
  3 + 4 = 7
  5 + 6 = 11
  7 + 8 = 15

合并中间结果:
[3, 7, 11, 15]

再次归约:
  3 + 7 = 10
  11 + 15 = 26

最终结果:
  10 + 26 = 36
```

reduce可以用分治实现，也可以用tbb的reduce接口实现, 用并行实现reduce的好处就是可以减少因“数据累计过多导致的浮点误差”，详见 [parallel_reduce.cpp](./parallel_reduce.cpp)

### parallel_deterministic_reduce 的特点

`parallel_deterministic_reduce` 和 `parallel_reduce` 都是 Intel TBB (Threading Building Blocks) 库中的并行算法，用于在并行环境中进行归约操作。它们的主要区别在于结果的确定性和一致性：

1. **parallel_reduce**:

   - 用于并行归约操作。
   - 结果可能是不确定的，因为不同的线程可能以不同的顺序执行操作。
   - 适用于不需要确定性结果的场景。

2. **parallel_deterministic_reduce**:
   - 也是用于并行归约操作。
   - 保证结果的确定性，即无论多少次运行，结果都是一致的。
   - 适用于需要确定性结果的场景。

简单来说，`parallel_deterministic_reduce` 在并行归约操作中提供了确定性，而 `parallel_reduce` 则不保证结果的一致性。
`parallel_deterministic_reduce` 提供确定性的原因在于它在并行归约操作中确保了操作的顺序和组合方式的一致性。具体来说，它通过以下方式实现确定性：

1. **固定的任务划分**：`parallel_deterministic_reduce` 会以固定的方式将任务划分给不同的线程，这样每次运行时任务划分都是相同的。
2. **固定的归约顺序**：在归约操作中，`parallel_deterministic_reduce` 保证了每次归约操作的顺序是固定的，不会因为线程调度的不同而改变。

这种确定性对于某些应用场景非常重要，例如需要重复运行并比较结果的科学计算、金融计算等。通过使用 `parallel_deterministic_reduce`，可以确保每次运行的结果都是一致的，从而避免由于并行执行顺序不同而导致的结果不一致问题。

## 扫描 (scan)

在并行计算中，`scan`（也称为前缀和或前缀扫描）是一种常见的并行算法，用于计算一个数组的前缀和。具体来说，`scan` 操作会生成一个新的数组，其中每个元素是原数组中从起始位置到当前元素的累加和。

例如，给定一个数组 `[a, b, c, d]`，`scan` 操作的结果是 `[a, a+b, a+b+c, a+b+c+d]`。

`scan` 操作有两种主要形式：

1. **Inclusive Scan**：包括当前元素在内的前缀和。例如，`[a, a+b, a+b+c, a+b+c+d]`。
2. **Exclusive Scanjj**：不包括当前元素的前缀和。例如，`[0, a, a+b, a+b+c]`。

在并行计算中，`scan` 操作可以通过分治法高效地实现。它通常分为两个阶段：

1. **上升阶段（Reduction Phase）**：构建一个树结构，从底部向顶部累加部分和。
2. **下降阶段（Downsweep Phase）**：从顶部向底部传播累加和，计算最终的前缀和。

代码详见: [parallel_scan.cpp](./parallel_scan.cpp)

## 性能测试

TODO
