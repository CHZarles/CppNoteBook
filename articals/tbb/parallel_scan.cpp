#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <tbb/parallel_scan.h>
#include <tbb/task_group.h>
#include <vector>

void scan() {

  size_t n = 1 << 26;
  std::vector<float> a(n);
  float res = 0;

  for (size_t i = 0; i < n; ++i) {
    res += std::sin(i);
    a[i] = res;
  }

  std::cout << a[n / 2] << std::endl;
  std::cout << res << std::endl;
}

void case0() {
  /*
   * 第一步、4个线程，每人处理2个元素的缩并
   * 第二步、1个线程，独自处理3个元素的缩并
   * 第三步、3个线程，每人处理2个元素的缩并
   */
  size_t n = 1 << 26; // 图片是8个
  std::vector<float> a(n);
  float res = 0;

  size_t maxt = 4;
  tbb::task_group tg1;

  // 第一步、4个线程，每人处理2个元素的缩并
  // 保存第一步运算的结果
  std::vector<float> tmp_res(maxt);
  for (size_t t = 0; t < maxt; t++) {
    size_t beg = t * n / maxt;
    size_t end = std::min(n, (t + 1) * n / maxt);
    tg1.run([&, t, beg, end] {
      float local_res = 0;
      for (size_t i = beg; i < end; ++i) {
        local_res += std::sin(i);
      }
      tmp_res[t] = local_res;
    });
  }
  // 执行完之后
  // tmp_res[0] = a[0] + .... a[n/4-1]
  // tmp_res[1] = a[n/4] + .... a[n/2-1]
  // tmp_res[2] = a[n/2] + .... a[3n/4-1]
  // tmp_res[3] = a[3n/4] + .... a[n-1]

  // 第二步、1个线程，独自处理3个元素的缩并
  tg1.wait();
  for (size_t t = 0; t < maxt; t++) {
    tmp_res[t] += res;
    res = tmp_res[t];
  }
  // 执行完之后
  // tmp_res[0] = a[0] + .... a[n/4-1]
  // tmp_res[1] = a[0] + .... a[n/2-1]
  // tmp_res[2] = a[0] + .... a[3n/4-1]
  // tmp_res[3] = a[0] + .... a[n-1]

  // 第三步、3个线程，每人处理2个元素的缩并
  tbb::task_group tg2;
  for (size_t t = 1; t < maxt; t++) { // t start from 1
    size_t beg = t * n / maxt;
    size_t end = std::min(n, (t + 1) * n / maxt) - 1;
    tg2.run([&, t, beg, end] {
      float local_res = tmp_res[t];
      for (size_t i = beg; i < end; i++) {
        local_res += std::sin(i);
        a[i] = local_res;
      }
    });
  }
  tg2.wait();

  std::cout << a[n / 2] << std::endl;
  std::cout << res << std::endl;
}

void case1() {
  size_t n = 1 << 26;
  std::vector<float> a(n);
  float res = tbb::parallel_scan(
      tbb::blocked_range<size_t>(0, n), 0.f,
      [&](const tbb::blocked_range<size_t> &r, float sum,
          bool is_final_scan) -> float {
        float local_res = sum;
        for (size_t i = r.begin(); i < r.end(); i++) {
          local_res += std::sin(i);
          if (is_final_scan) {
            a[i] = local_res;
          }
        }
        return local_res;
      },
      std::plus<float>());
  std::cout << a[n / 2] << std::endl;
  std::cout << res << std::endl;
}
int main() {
  TimeCost(scan, "scan");
  TimeCost(case0, "case0");
  TimeCost(case1, "case1");
  return 0;
}
