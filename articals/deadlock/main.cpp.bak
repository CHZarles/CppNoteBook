#include <iostream>
#include <mutex>
#include <thread>
#include <vector>

int main() {
  std::vector<int> arr1;
  std::mutex mtx1;
  std::vector<int> arr2;
  std::mutex mtx2;
  // sample 0
  {

    std::thread t1([&] {
      for (int i = 0; i < 100000; ++i) {
        {
          std::lock_guard grd(mtx1);
          arr1.push_back(i);
        }
        {
          std::lock_guard grd(mtx2);
          arr2.push_back(i);
        }
      }
    });

    std::thread t2([&] {
      for (int i = 0; i < 100000; ++i) {
        {
          std::lock_guard grd(mtx1);
          arr1.push_back(i);
        }
        {
          std::lock_guard grd(mtx2);
          arr2.push_back(i);
        }
      }
    });
    t1.join();
    t2.join();
  }

  // sample 1
  {
    if (mtx1.try_lock())
      printf("sucessed\n");
    else
      printf("failed \n");

    if (mtx1.try_lock())
      printf("sucessed\n");
    else
      printf("failed \n");

    mtx1.unlock();
  }

  // sample 2
  {
    std::timed_mutex mtx;
    if (mtx.try_lock_for(std::chrono::milliseconds(500)))
      printf("sucessed\n");
    else
      printf("failed \n");

    if (mtx.try_lock_for(std::chrono::milliseconds(500)))
      printf("sucessed\n");
    else
      printf("failed \n");

    mtx.unlock();
  }
  return 0;
}
