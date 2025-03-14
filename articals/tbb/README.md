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

### 循环：parallel_for

#### 用法1

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

#### 用法2
