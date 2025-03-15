#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <tbb/blocked_range2d.h>
#include <tbb/blocked_range3d.h>
#include <tbb/parallel_for.h>
#include <tbb/parallel_for_each.h>
#include <vector>

// 形式 1
void case0() {
  size_t n = 1 << 26;
  std::vector<float> a(n);
  // 推荐这种？编译器有可能会优化
  tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
                    [&](const tbb::blocked_range<size_t> &r) {
                      for (size_t i = r.begin(); i != r.end(); ++i) {
                        a[i] = std::sqrt(a[i]);
                      }
                    });
}

void case1() {
  size_t n = 1 << 26;
  std::vector<float> a(n);
  tbb::parallel_for((size_t)0, (size_t)n,
                    [&](size_t i) { a[i] = std::sqrt(a[i]); });
}

void case2() {
  size_t n = 1 << 26;
  std::vector<float> a(n);

  tbb::parallel_for_each(a.begin(), a.end(), [&](float &f) { f = 32.f; });
}

// 2d parallel
void case3() {
  size_t n = 1 << 13;
  std::vector<float> a(n * n);
  tbb::parallel_for(
      tbb::blocked_range2d<size_t>(0, n, 0, n),
      [&](tbb::blocked_range2d<size_t> r) {
        for (size_t i = r.cols().begin(); i < r.cols().end(); ++i) {
          for (size_t j = r.rows().begin(); j < r.rows().end(); ++j) {
            a[i * n + j] = std::sin(i) * std::sin(j);
          }
        }
      });
}

// 3d parallel
void case4() {
  size_t n = 1 << 6;
  std::vector<float> a(n * n * n);
  tbb::parallel_for(
      tbb::blocked_range3d<size_t>(0, n, 0, n, 0, n),
      [&](tbb::blocked_range3d<size_t> r) {
        for (size_t i = r.pages().begin(); i < r.pages().end(); ++i) {
          for (size_t j = r.rows().begin(); j < r.rows().end(); ++j) {
            for (size_t k = r.cols().begin(); k < r.cols().end(); ++k) {
              a[i * n * n + j * n + k] =
                  std::sin(i) * std::sin(j) * std::sin(k);
            }
          }
        }
      });
}

int main() {
  TimeCost(case0, "case0");
  TimeCost(case1, "case1");
  TimeCost(case2, "case2");
  TimeCost(case3, "case3");
  TimeCost(case4, "case4");
  return 0;
}
