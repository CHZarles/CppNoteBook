#include <chrono>
#include <iostream>
#include <thread>

void case0() {
  auto t0 = std::chrono::steady_clock::now();
  for (volatile int i = 0; i < 1e7; ++i) {
  }
  auto t1 = std::chrono::steady_clock::now();
  auto dt = t1 - t0;
  {

    int64_t ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(dt).count();
    std::cout << "Time: " << ms << " ms" << std::endl;
  }

  {
    using double_ms = std::chrono::duration<double, std::milli>;
    double ms = std::chrono::duration_cast<double_ms>(dt).count();
    std::cout << "Time: " << ms << " ms" << std::endl;
  }
}

void case1() { std::this_thread::sleep_for(std::chrono::milliseconds(400)); }

void case2() {
  auto t = std::chrono::steady_clock::now() + std::chrono::milliseconds(400);
  std::this_thread::sleep_until(t);
}
int main() {
  case0();
  case1();
  case2();
  return 0;
}
