# std::condition_variable

条件变量用来控制线程同步，必须和 std::unique_lock\<std::mutex\> 配合使用。

用下面例子举例

```c++
void case0() {
  std::condition_variable cv;
  std::mutex mtx;

  std::thread t1([&]() {
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock);
    std::cout << "Thread 1 is awake" << std::endl;
  });

  // do something

  std::cout << "notifying..." << std::endl;
  cv.notify_one();

  t1.join();
}
```

- cv.wait(lck) 将会让当前线程陷入等待。
- 在其他线程中调用 cv.notify_one() 则会唤醒那个陷入等待的线程。

## 配置 wait 的唤醒条件

还可以额外指定一个参数，变成 cv.wait(lck, expr) 的形式，其中 expr 是个 lambda 表达式，只有其返回值为 true 时才会真正唤醒，否则继续等待。

```c++

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
```

## 条件变量控制多个等待者

```c++

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

```

cv.notify_one() 只会唤醒其中一个等待中的线程，而 cv.notify_all() 会唤醒全部。

这就是为什么 wait() 需要一个 unique_lock 作为参数，因为要保证多个线程被唤醒时，只有一个能够被启动。如果不需要，在 wait() 返回后调用 lck.unlock() 即可。

顺便一提，wait() 的过程中会暂时 unlock() 这个锁。

## std::condition_variable 小贴士

- std::condition_variable 仅仅支持 std::unique_lock<std::mutex> 作为 wait 的参数，如果需要用其他类型的 mutex 锁，可以用 std::condition_variable_any。

- 他还有 wait_for() 和 wait_until() 函数，分别接受 chrono 时间段和时间点作为参数。详见：https://en.cppreference.com/w/cpp/thread/condition_variable/wait_for。
