# 同时锁住多个 mutex：死锁难题

一个死锁的 case

```c++
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

int main() {
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
  return 0;
}

```

由于同时执行的两个线程，他们中发生的指令不一定是同步的，因此有可能出现这种情况：

- t1 执行 mtx1.lock()。
- t2 执行 mtx2.lock()。
- t1 执行 mtx2.lock()：失败，陷入等待
- t2 执行 mtx1.lock()：失败，陷入等待
  双方都在等着对方释放锁，但是因为等待而无法释放锁，从而要无限制等下去。
  这种现象称为死锁（dead-lock）。

## 解决1：永远不要同时持有两个锁

- 最为简单的方法，就是一个线程永远不要同时持有两个锁，分别上锁，这样也可以避免死锁。
- 因此这里双方都在 mtx1.unlock() 之后才 mtx2.lock()，从而也不会出现一方等着对方的同时持有了对方等着的锁的情况。

```c++

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

```

## 解决2：保证双方上锁顺序一致

- 其实，只需保证双方上锁的顺序一致，即可避免死锁。因此这里调整 t2 也变为先锁 mtx1，再锁 mtx2。
- 这时，无论实际执行顺序是怎样，都不会出现一方等着对方的同时持有了对方等着的锁的情况。

```c++
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

```

## 解决3：用 std::lock 同时对多个上锁

- 如果没办法保证上锁顺序一致，可以用标准库的 std::lock(mtx1, mtx2, ...) 函数，一次性对多个 mutex 上锁。
- 他接受任意多个 mutex 作为参数，并且他保证在无论任意线程中调用的顺序是否相同，都不会产生死锁问题。

```c++

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

```

### std::lock 的 RAII 版本：std::scoped_lock

和 std::lock_guard 相对应，std::lock 也有 RAII 的版本 std::scoped_lock。只不过他可以同时对多个 mutex 上锁。

```c++

  // std::lock 同时上多个锁
  std::thread t1([&]() {
    for (int i = 0; i < 1000000; i++) {
      std::scoped_lock(mtx1, mtx2);
      // do something
    }
  });

  std::thread t2([&]() {
    for (int i = 0; i < 1000000; i++) {
      std::scoped_lock(mtx1, mtx2);
      // do something
    }
  });

```

# 同一个线程重复调用 lock() 也会造成死锁

- 除了两个线程同时持有两个锁会造成死锁外，即使只有一个线程一个锁，如果 lock() 以后又调用 lock()，也会造成死锁。

```c++
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
```

## 解决1：改用 std::recursive_mutex

可以用 std::recursive_mutex。他会自动判断是不是同一个线程 lock() 了多次同一个锁，如果是则让计数器加1，之后 unlock() 会让计数器减1，减到0时才真正解锁。但是相比普通的 std::mutex 有一定性能损失。

```c++
void solution1() {
  std::recursive_mutex mtx1;
  mtx1.lock();
  auto other = [&]() {
    mtx1.lock();
    // do something
    mtx1.unlock();
  };
  other();
  mtx1.unlock();
}
```
