#include <cmath>
#include <iostream>
#include <tbb/parallel_for.h>
#include <tbb/task_arena.h>
#include <vector>

int main() {
  size_t n = 1 << 26;
  std::vector<float> a(n);

  // 任务域名 指定四个线程
  tbb::task_arena ta(4);
  // 指定任务域
  ta.execute([&] {
    tbb::parallel_for((size_t)0, (size_t)n,
                      [&](size_t i) { a[i] = std::sin(i); });
  });

  return 0;
}
