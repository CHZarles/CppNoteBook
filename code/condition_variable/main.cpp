#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>

void case0() {

  std::condition_variable cv;
  std::mutex mtx;

  std::thread t1([&]() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);
    std::cout << "Thread 1 is awake" << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(400));

  std::cout << "notifying..." << std::endl;
  cv.notify_one();

  t1.join();
}

void case1() {

  std::condition_variable cv;
  std::mutex mtx;
  bool ready = false;

  std::thread t1([&]() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [&] { return ready; });
    std::cout << "Thread 1 is awake" << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(400));

  std::cout << "notifying..." << std::endl;
  cv.notify_one(); // useless , because ready is false

  std::cout << "notifying..." << std::endl;
  ready = true;
  cv.notify_one(); // awake t1
  t1.join();
}

void case2() {
  std::condition_variable cv;
  std::mutex mtx;

  std::thread t1([&] {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);
    std::cout << "Thread 1 is awake" << std::endl;
  });
  std::thread t2([&] {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);
    std::cout << "Thread 2 is awake" << std::endl;
  });
  std::thread t3([&] {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);
    std::cout << "Thread 3 is awake" << std::endl;
  });

  std::this_thread::sleep_for(std::chrono::milliseconds(400));

  std::cout << "notifying one\n";
  cv.notify_one(); // awake t1 only

  std::this_thread::sleep_for(std::chrono::milliseconds(400));
  std::cout << "notifying all\n";
  cv.notify_all(); // awake t2 and t3

  t1.join();
  t2.join();
  t3.join();
}

void case3() {
  std::condition_variable cv;
  std::mutex mtx;
  std::vector<int> foods;

  std::thread t1([&] {
    for (int i = 0; i < 2; i++) {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [&] { return !foods.empty(); });
      auto food = foods.back();
      foods.pop_back();
      lock.unlock();
      std::cout << "t1 get food: " << food << std::endl;
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 2; i++) {
      std::unique_lock<std::mutex> lock(mtx);
      cv.wait(lock, [&] { return !foods.empty(); });
      auto food = foods.back();
      foods.pop_back();
      lock.unlock();
      std::cout << "t2 get food: " << food << std::endl;
    }
  });

  foods.push_back(12);
  cv.notify_one();
  foods.push_back(13);
  cv.notify_one();
  foods.push_back(14);
  foods.push_back(15);
  cv.notify_all();

  t1.join();
  t2.join();
}

template <typename T> class MTQueue {
  std::condition_variable m_cv;
  std::mutex m_mtx;
  std::vector<T> m_arr;

public:
  T pop() {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this] { return !m_arr.empty(); });
    T ret = std::move(m_arr.back());
    m_arr.pop_back();
    return ret;
  }

  auto pop_hold() {
    std::unique_lock<std::mutex> lock(m_mtx);
    m_cv.wait(lock, [this] { return !m_arr.empty(); });
    T ret = std::move(m_arr.back());
    m_arr.pop_back();
    return std::pair{std::move(ret), std::move(lock)};
  }

  void push(const T &val) {
    std::unique_lock lck(m_mtx);
    m_arr.push_back(std::move(val));
    m_cv.notify_one();
  }

  void push_many(std::initializer_list<T> vals) {
    std::unique_lock lck(m_mtx);
    std::copy(std::move_iterator(vals.begin()), std::move_iterator(vals.end()),
              std::back_inserter(m_arr));
    m_cv.notify_all();
  }
};
void case4() {

  MTQueue<int> foods;
  std::thread t1([&] {
    for (int i = 0; i < 2; i++) {
      auto food = foods.pop();
      std::cout << "t1 get food: " << food << std::endl;
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 2; i++) {
      auto food = foods.pop();
      std::cout << "t2 get food: " << food << std::endl;
    }
  });

  foods.push(12);
  foods.push(13);
  foods.push_many({14, 15});
  t1.join();
  t2.join();
}
int main() {
  case0();
  case1();
  case2();
  case3();
  case4();
  return 0;
}
