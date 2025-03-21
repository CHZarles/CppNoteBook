#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <tbb/parallel_invoke.h>
#include <tbb/parallel_sort.h>
#include <vector>
#pragma once

// #include <chrono>
// #define TICK(x) auto bench_##x = std::chrono::steady_clock::now();
// #define TOCK(x) std::cout << #x ": " <<
// std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::steady_clock::now()
// - bench_##x).count() << "s" << std::endl;

#include <tbb/tick_count.h>
#define TICK(x) auto bench_##x = tbb::tick_count::now();
#define TOCK(x)                                                                \
  std::cout << #x ": " << (tbb::tick_count::now() - bench_##x).seconds()       \
            << "s" << std::endl;

int fib(int n = 39) {
  if (n < 2)
    return n;
  int first, second;
  tbb::parallel_invoke([&] { first = fib(n - 1); },
                       [&] { second = fib(n - 2); });
  return first + second;
}

int serial_fib(int n) {
  if (n < 2)
    return n;
  int first = serial_fib(n - 1);
  int second = serial_fib(n - 2);
  return first + second;
}
int fib2(int n = 39) {
  if (n < 29)
    return serial_fib(n);
  int first, second;
  tbb::parallel_invoke([&] { first = fib2(n - 1); },
                       [&] { second = fib2(n - 2); });
  return first + second;
}

template <class T>
void quick_sort(T *data, size_t size) {
  if (size < 1)
    return;
  if (size < (1 << 16)) {
    std::sort(data, data + size, std::less<T>{});
    return;
  }
  size_t mid = std::hash<size_t>{}(size);
  mid ^= std::hash<void *>{}(static_cast<void *>(data));
  mid %= size;
  std::swap(data[0], data[mid]);
  T pivot = data[0];
  size_t left = 0, right = size - 1;
  while (left < right) {
    while (left < right && !(data[right] < pivot))
      right--;
    if (left < right)
      data[left++] = data[right];
    while (left < right && data[left] < pivot)
      left++;
    if (left < right)
      data[right--] = data[left];
  }
  data[left] = pivot;
  tbb::parallel_invoke([&] { quick_sort(data, left); },
                       [&] { quick_sort(data + left + 1, size - left - 1); });
}

void compare_sort() {
  size_t n = 1 << 24;
  { // std
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(std_sort);
    std::sort(arr.begin(), arr.end(), std::less<int>{});
    TOCK(std_sort);
  }

  { // handmake
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(quick_sort);
    quick_sort(arr.data(), arr.size());
    TOCK(quick_sort);
  }

  { // tbb
    std::vector<int> arr(n);
    std::generate(arr.begin(), arr.end(), std::rand);
    TICK(tbb_parallel_sort);
    tbb::parallel_sort(arr.begin(), arr.end(), std::less<int>{});
    TOCK(tbb_parallel_sort);
  }
}

int main() {
  TICK(fib);
  std::cout << fib(39) << std::endl;
  TOCK(fib);

  TICK(fib2);
  std::cout << fib2(39) << std::endl;
  TOCK(fib2);

  compare_sort();
}
