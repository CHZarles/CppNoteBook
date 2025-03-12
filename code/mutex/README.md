# 互斥量

## 多线程打架案例

```c++

#include <iostream>
#include <thread>
#include <vector>

int main() {
  std::vector<int> arr;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
      arr.push_back(i);
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 100000; ++i) {
      arr.push_back(i);
    }
  });

  t1.join();
  t2.join();
  std::cout << "arr.size() : " << arr.size();
  return 0;
}

```

vector 不是多线程安全（MT-safe）的容器

> malloc(): corrupted top size
> ./run.sh: line 6: 213643 Aborted (core dumped) build/cpptest

## std::mutex：上锁，防止多个线程同时进入某一代码段

- 调用 std::mutex 的 lock() 时，会检测 mutex 是否已经上锁。
- 如果没有锁定，则对 mutex 进行上锁。
- 如果已经锁定，则陷入等待，直到 mutex 被另一个线程解锁后，才再次上锁。而调用 unlock() 则会进行解锁操作。
- 这样，就可以保证 mtx.lock() 和 mtx.unlock() 之间的代码段，同一时间只有一个线程在执行，从而避免数据竞争。

```c++
+ std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
+     mtx.lock();
      arr.push_back(i);
+     mtx.unlock();
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 100000; ++i) {
+     mtx.lock();
      arr.push_back(i);
+     mtx.unlock();
    }
  });

```

## std::lock_guard：符合 RAII 思想的上锁和解锁

根据 RAII 思想，可将锁的持有视为资源，上锁视为锁的获取，解锁视为锁的释放。

std::lock_guard 就是这样一个工具类，他的构造函数里会调用 mtx.lock()，解构函数会调用 mtx.unlock()。从而退出函数作用域时能够自动解锁，避免程序员粗心不小心忘记解锁。

```c++

+ std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
+     std::lock_guard grd(mtx);
      arr.push_back(i);
    }
  });

  std::thread t2([&] {
    for (int i = 0; i < 100000; ++i) {
+     std::lock_guard grd(mtx);
      arr.push_back(i);
    }
  });

```

## std::unique_lock：也符合 RAII 思想，但自由度更高

std::lock_guard 严格在解构时 unlock()，但是有时候我们会希望提前 unlock()。

这时可以用 std::unique_lock，他额外存储了一个 flag 表示是否已经被释放。

他会在解构检测这个 flag，如果没有释放，则调用 std::unique_lock::unlock()，否则不调用。

```
+ std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
+     std::unique_lock grd(mtx,std::deter_lock);
      arr.push_back(i);
+     grd.unlock()
    }
  });

```

## std::unique_lock：用 std::defer_lock 作为参数

std::unique_lock 的构造函数还可以有一个额外参数，那就是 std::defer_lock。

```
+ std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
+     std::lock_guard grd(mtx,std::deter_lock);
+     grd.lock()
      arr.push_back(i);
+     grd.unlock()
    }
  });

```

指定了这个参数的话，std::unique_lock 不会在构造函数中调用 mtx.lock()，需要之后再手动调用 grd.lock() 才能上锁。

好处依然是即使忘记 grd.unlock() 也能够自动调用 mtx.unlock()。

> 可以看一下 std::defer_lock_t，是个空的类，其实这种用一个空 tag 类来区分不同构造函数的思想在 C++ 中很常见，包括std::inplace, std::piecewise_construct 等

## 多个对象，多个mutex

mtx1 用来锁定 arr1，mtx2 用来锁定 arr2。

不同的对象，各有一个 mutex，独立地上锁，可以避免不必要的锁定，提升高并发时的性能。

还用了一个 {} 包住 std::lock_guard，限制其变量的作用域，从而可以让他在 } 之前解构并调用 unlock()，也避免了和下面一个 lock_guard 变量名冲突。

```c++
  std::vector<int> arr1;
  std::mutex mtx1;
  std::vector<int> arr2;
  std::mutex mtx2;

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
    // ... as same as above
    }
  });

```

## 如果上锁失败，不要等待：try_lock()

我们说过 lock() 如果发现 mutex 已经上锁的话，会等待他直到他解锁。

也可以用无阻塞的 try_lock()，他在上锁失败时不会陷入等待，而是直接返回 false；如果上锁成功，则会返回 true。

比如下面这个例子，第一次上锁，因为还没有人上锁，所以成功了，返回 true。

第二次上锁，由于自己已经上锁，所以失败了，返回 false。

```c++

    if (mtx1.try_lock())
      printf("sucessed\n");
    else
      printf("failed \n");

    if (mtx1.try_lock())
      printf("sucessed\n");
    else
      printf("failed \n");

    mtx1.unlock();
```

## 只等待一段时间：try_lock_for()

try_lock() 碰到已经上锁的情况，会立即返回 false。

如果需要等待，但仅限一段时间，可以用 std::timed_mutex 的 try_lock_for() 函数，他的参数是最长等待时间，同样是由 chrono 指定时间单位。超过这个时间还没成功就会“不耐烦地”失败并返回 false；如果这个时间内上锁成功则返回 true。

同理还有接受时间点的 try_lock_until()。

```c++

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

```

## std::unique_lock：用 std::try_to_lock 做参数

和无参数相比，他会调用 mtx1.try_lock() 而不是 mtx1.lock()。之后，可以用 grd.owns_lock() 判断是否上锁成功。

```c++

  std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
      std::unique_lock grd(mtx,std::try_to_lock);
      if (grd.owns_lock()){
          printf("lock success\n");
          arr.push_back(i);
      }
    }
  });

```

## std::unique_lock：用 std::adopt_lock 做参数

如果当前 mutex 已经上锁了，但是之后仍然希望用 RAII 思想在解构时候自动调用 unlock()，可以用 std::adopt_lock 作为 std::unique_lock 或 std::lock_guard 的第二个参数，这时他们会默认 mtx 已经上锁

> `std::adopt_lock` is used with `std::unique_lock` to indicate that the mutex is already locked by the current thread and should be adopted without attempting to lock it again.

```c++
  std::mutex mtx;
  std::thread t1([&] {
    for (int i = 0; i < 100000; ++i) {
      mtx.lock(); // mtx lock
      std::unique_lock grd(mtx,std::adopt_lock);
      ...
    }
  });

```

## std::unique_lock 和 std::mutex 具有同样的接口

其实 std::unique_lock 具有 mutex 的所有成员函数：lock(), unlock(), try_lock(), try_lock_for() 等。

因为 std::lock_guard 无非是调用其构造参数名为 lock() 的成员函数，所以 std::unique_lock 也可以作为 std::lock_guard 的构造参数！

这种只要具有某些指定名字的成员函数，就判断一个类是否满足某些功能的思想，在 Python 称为**鸭子类型**, 而 C++ 称为 concept（概念）。比起虚函数和动态多态的接口抽象，concept 使实现和接口更加解耦合且没有性能损失。
