#include <benchmark/benchmark.h>
#include <cmath>
#include <iostream>
#include <vector>
#include <tbb/parallel_reduce.h>
#include <tbb/parallel_for.h>

constexpr size_t n = 1 << 27;
std::vector<float> a(n);

void BM_for(benchmark::State &bm) {
  for (auto _ : bm) {
    // fill a with sin(i)
    for (size_t i = 0; i < n; i++) {
      a[i] = std::sin(i);
    }
  }
}

BENCHMARK(BM_for);

void BM_tbb_for(benchmark::State &bm){
  for (auto _ : bm) {

  tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
                    [&](const tbb::blocked_range<size_t> &r) {
                      for (size_t i = r.begin(); i != r.end(); ++i) {
                        a[i] = std::sin(i);
                      }
                    });
    }
}

BENCHMARK(BM_tbb_for);

void BM_reduce(benchmark::State &bm) {
  for (auto _ : bm) { // 框架会自动决定要重复多少次
    // calculate sum of a
    float res = 0;
    for (size_t i = 0; i < n; i++) {
      res += a[i];
    }
    // 让编译器不要优化掉res
    benchmark::DoNotOptimize(res);
  }
}

BENCHMARK(BM_reduce);


void BM_tbb_reduce(benchmark::State &bm) {
  for (auto _ : bm) { // 框架会自动决定要重复多少次
    float res = tbb::parallel_reduce(
      tbb::blocked_range<size_t>(0, n),
      (float)0,
      [&](tbb::blocked_range<size_t> r, float local_res) {
        for (size_t i = r.begin(); i < r.end(); ++i) {
          local_res += a[i];
        }
        return local_res;
      },
      [](float x, float y) {
        { return x + y; }
      });

  }
}

BENCHMARK(BM_tbb_reduce);


BENCHMARK_MAIN();
