#include <cmath>
#include <iostream>
#include <tbb/concurrent_vector.h>
#include <tbb/parallel_for.h>
#include <vector>
#include "utils.hpp"
size_t n = 1<<27;


// 串行
void case0(){

  std::vector<float> a;

  for (size_t i = 0; i < n; i++) {
      float val = std::sin(i);
      if (val > 0) {
          a.push_back(val);
      }
  }
}

// 优化方案1
void case1(){
    tbb::concurrent_vector<float> a;

    tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
    [&] (tbb::blocked_range<size_t> r) {
        for (size_t i = r.begin(); i < r.end(); i++) {
            float val = std::sin(i);
            if (val > 0) {
                a.push_back(val);
            }
        }
    });

}

void case2(){
  tbb::concurrent_vector<float> a;

  tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
  [&] (tbb::blocked_range<size_t> r) {
      std::vector<float> local_a;
      for (size_t i = r.begin(); i < r.end(); i++) {
          float val = std::sin(i);
          if (val > 0) {
              local_a.push_back(val);
          }
      }
      auto it = a.grow_by(local_a.size());
      for (size_t i = 0; i < local_a.size(); i++) {
          *it++ = local_a[i];
      }
  });
}


void case3(){
  tbb::concurrent_vector<float> a;

    tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
    [&] (tbb::blocked_range<size_t> r) {
        std::vector<float> local_a;
        local_a.reserve(r.size());
        for (size_t i = r.begin(); i < r.end(); i++) {
            float val = std::sin(i);
            if (val > 0) {
                local_a.push_back(val);
            }
        }
        auto it = a.grow_by(local_a.size());
        std::copy(local_a.begin(), local_a.end(), it);
    });

}

void case4(){
  std::vector<float> a;
  std::mutex mtx; // 这里其实也可以换成 tbb::spin_mutex

  a.reserve(n * 2 / 3);
  tbb::parallel_for(tbb::blocked_range<size_t>(0, n),
  [&] (tbb::blocked_range<size_t> r) {
      std::vector<float> local_a;
      local_a.reserve(r.size());
      for (size_t i = r.begin(); i < r.end(); i++) {
          float val = std::sin(i);
          if (val > 0) {
              local_a.push_back(val);
          }
      }
      std::lock_guard lck(mtx);
      std::copy(local_a.begin(), local_a.end(), std::back_inserter(a));
  });

}

int main() {
  std::cout << "parallel filter test\n";
  TimeCost(case0,"case0");
  TimeCost(case1, "case1");
  TimeCost(case2,"case2");
  TimeCost(case3,"case3");
  TimeCost(case4, "casr4");
}
