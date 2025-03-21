#include <cmath>
#include <iostream>
#include <mutex>
#include <tbb/parallel_for.h>
#include <vector>

// maybe
void dead_lock_case() {
  std::cout << "dead_lock_case start\n";
  size_t n = 1 << 10;
  std::vector<float> a(n * n);
  std::mutex mtx;

  tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
    tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
      std::lock_guard<std::mutex> lock(mtx);
      a[i * n + j] = std::sin(i) * std::cos(j);
    });
  });
  std::cout << "dead_lock_case done\n";
}

void solution0() {

  std::cout << "solution0 start" << std::endl;
  size_t n = 1 << 10;
  std::vector<float> a(n * n);
  std::recursive_mutex mtx;

  tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
    tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
      std::lock_guard lock(mtx);
      a[i * n + j] = std::sin(i) * std::cos(j);
    });
  });

  std::cout << "solution0 done" << std::endl;
}

void solution1() {
  std::cout << "solution1 start" << std::endl;
  size_t n = 1 << 10;
  std::vector<float> a(n * n);
  std::mutex mtx;

  tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
    std::lock_guard<std::mutex> lock(mtx);
    tbb::task_arena ta;
    ta.execute([&] {
      tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
        a[i * n + j] = std::sin(i) * std::cos(j);
      });
    });
  });
  std::cout << "solution1 done" << std::endl;
}

void solution2() {
  std::cout << "solution2 start" << std::endl;
  size_t n = 1 << 10;
  std::vector<float> a(n * n);
  std::mutex mtx;

  tbb::parallel_for((size_t)0, (size_t)n, [&](size_t i) {
    std::lock_guard<std::mutex> lock(mtx);
    tbb::this_task_arena::isolate([&] {
      tbb::parallel_for((size_t)0, (size_t)n, [&](size_t j) {
        a[i * n + j] = std::sin(i) * std::cos(j);
      });
    });
  });
  std::cout << "solution2 done" << std::endl;
}
int main() {
  solution0();
  solution1();
  solution2();
  // dead_lock_case();
}
