#include "utils.hpp"
#include <cmath>
#include <tbb/blocked_range2d.h>
#include <tbb/parallel_reduce.h>
#include <tbb/task_group.h>
#include <vector>

// 顺序运行版本
void case0() {
  size_t n = 1 << 26;
  float res = 0;
  for (size_t i = 0; i < n; ++i) {
    res += std::sin(i);
  }

  std::cout << res << std::endl;
}

// 分治版本
void case1() {
  size_t n = 1 << 26;
  float res = 0;

  size_t maxt = 4;
  tbb::task_group tg;
  std::vector<float> tmp_res(maxt);
  for (size_t t = 0; t < maxt; ++t) {
    size_t beg = t * n / maxt;
    size_t end = std::min(n, (t + 1) * n / maxt);
    tg.run([&, t, beg, end] {
      float local_res = 0;
      for (size_t i = beg; i < end; ++i) {
        local_res += std::sin(i);
      }
      tmp_res[t] = local_res;
    });
  }
  tg.wait();
  for (size_t t = 0; t < maxt; ++t) {
    res += tmp_res[t];
  }
  std::cout << res << std::endl;
}

// reduce 版本
void case2() {
  size_t n = 1 << 26;
  float res = tbb::parallel_reduce(
      // Define the range to be processed
      tbb::blocked_range<size_t>(0, n),
      // Initial value for the reduction
      (float)0,
      // Lambda function to process each sub-range
      [&](tbb::blocked_range<size_t> r, float local_res) {
        // std::cout << r.begin() << " " << r.end() << std::endl;
        for (size_t i = r.begin(); i < r.end(); ++i) {
          // local_res += std::sin(i);
          local_res += i;
        }
        return local_res;
      },
      // Lambda function to combine the results of sub-ranges
      [](float x, float y) {
        { return x + y; }
      });
  std::cout << res << std::endl;
}

// parallel_deterministic_reduce
void case3() {
  size_t n = 1 << 26;
  float res = tbb::parallel_deterministic_reduce(
      // Define the range to be processed
      tbb::blocked_range<size_t>(0, n),
      // Initial value for the reduction
      (float)0,
      // Lambda function to process each sub-range
      [&](tbb::blocked_range<size_t> r, float local_res) {
        // std::cout << r.begin() << " " << r.end() << std::endl;
        for (size_t i = r.begin(); i < r.end(); ++i) {
          // local_res += std::sin(i);
          local_res += i;
        }
        return local_res;
      },
      // Lambda function to combine the results of sub-ranges
      [](float x, float y) {
        { return x + y; }
      });
  std::cout << res << std::endl;
}

int main() {
  TimeCost(case0, "case0");
  TimeCost(case1, "case1");
  TimeCost(case2, "case2");
  TimeCost(case3, "case3");
}
