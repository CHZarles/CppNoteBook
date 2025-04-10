
#include <benchmark/benchmark.h>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

constexpr size_t n = 1 << 27;
std::vector<float> a(n);

void BM_iter_int(benchmark::State &bm) {
  for (auto _ : bm) {
    // fill a with sin(i)
    for (int i = 0; i < n; i++) {
      a[i] = std::sin(i);
    }
  }
}

BENCHMARK(BM_iter_int);

void BM_iter_size_t(benchmark::State &bm) {
  for (auto _ : bm) {
    // fill a with sin(i)
    for (std::size_t i = 0; i < n; i++) {
      a[i] = std::sin(i);
    }
  }
}

BENCHMARK(BM_iter_size_t);

void BM_iter_loop_4(benchmark::State &bm) {
  for (auto _ : bm) {
    // fill a with sin(i)
    size_t n = 1024 * 4;
    for (std::size_t i = 0; i < n; i++) {
      a[i] = std::sin(i);
    }
  }
}
BENCHMARK(BM_iter_loop_4);

void BM_iter_loop_not_4(benchmark::State &bm) {
  for (auto _ : bm) {
    // fill a with sin(i)
    size_t n = 1022 * 4 - 1;
    for (std::size_t i = 0; i < n; i++) {
      a[i] = std::sin(i);
    }
  }
}
BENCHMARK(BM_iter_loop_not_4);
BENCHMARK_MAIN();
