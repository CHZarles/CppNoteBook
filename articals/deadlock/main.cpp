#include <iostream>
#include <mutex>
#include <string>
#include <thread>

// dead lock case 0
void dead_lock_case0() {

  printf("enter dead lock case 0\n");
  std::mutex mtx1, mtx2;

  // dead lock situation
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; i++) {
      mtx1.lock();
      mtx2.lock();
      mtx2.unlock();
      mtx1.unlock();
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000000; i++) {
      mtx2.lock();
      mtx1.lock();
      mtx1.unlock();
      mtx2.unlock();
    }
  });

  t1.join();
  t2.join();
}

void solution1() {

  std::mutex mtx1, mtx2;
  // t1, t2 各个线程同时只持有一个锁
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; i++) {
      mtx1.lock();
      mtx1.unlock();
      mtx2.lock();
      mtx2.unlock();
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000000; i++) {
      mtx2.lock();
      mtx2.unlock();
      mtx1.lock();
      mtx1.unlock();
    }
  });

  t1.join();
  t2.join();
  printf("solution1 run sucess\n");
}

void solution2() {

  std::mutex mtx1, mtx2;

  //  t1,t2 上锁顺序一致
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; i++) {
      mtx1.lock();
      mtx2.lock();
      mtx2.unlock();
      mtx1.unlock();
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000000; i++) {
      mtx1.lock();
      mtx2.lock();
      mtx2.unlock();
      mtx1.unlock();
    }
  });

  t1.join();
  t2.join();
  printf("solution2 run sucess\n");
}

void solution3() {
  std::mutex mtx1, mtx2;

  // std::lock 同时上多个锁
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; i++) {
      std::lock(mtx1, mtx2);
      mtx2.unlock();
      mtx1.unlock();
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000000; i++) {
      std::lock(mtx1, mtx2);
      mtx2.unlock();
      mtx1.unlock();
    }
  });

  t1.join();
  t2.join();
  printf("solution3 run sucess\n");
}

// dead_lock_case1
void dead_lock_case1() {
  printf("enter dead lock case 1\n");
  std::mutex mtx1;
  mtx1.lock();

  auto other = [&]() {
    mtx1.lock();
    // do something
    mtx1.unlock();
  };

  other();

  mtx1.unlock();
}

void solution4() {
  std::recursive_mutex mtx1;
  mtx1.lock();
  auto other = [&]() {
    mtx1.lock();
    // do something
    mtx1.unlock();
  };
  other();
  mtx1.unlock();
  printf("solution4 run sucess\n");
}

int main() {

  // dead_lock_case0()
  solution1();
  solution2();
  solution3();

  // dead_lock_case1();
  solution4();
  return 0;
}
