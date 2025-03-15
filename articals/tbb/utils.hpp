
#include <chrono>
#include <iostream>

void TimeCost(void (*func)(), const std::string &name) {
  auto start = std::chrono::high_resolution_clock::now();
  func();
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end - start;
  std::cout << name << " time cost: " << diff.count() << " s" << std::endl;
}
