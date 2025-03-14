#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

class MTVector {
  std::vector<int> m_arr;
  mutable std::shared_mutex m_mtx;

public:
  void push_back(int val) {
    std::lock_guard lock(m_mtx);
    m_arr.push_back(val);
  }

  size_t size() const {
    std::shared_lock grd(m_mtx);
    return m_arr.size();
  }
};

int main() {
  MTVector arr;

  std::thread t1([&]() {
    for (int i = 0; i < 1000; ++i) {
      arr.push_back(i);
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000; ++i) {
      arr.push_back(1000 + i);
    }
  });

  t1.join();
  t2.join();
  std::cout << arr.size() << "\n";
}
