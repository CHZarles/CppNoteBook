# 多线程环境使用 std::vector

## 最终代码

```c++

#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

class MTVector {
  std::vector<int> m_arr;
  // 知识点 0, shared_mutex , mutable
  mutable std::shared_mutex m_mtx;

public:
  void push_back(int val) {
    std::lock_guard lock(m_mtx);
    m_arr.push_back(val);
  }

  size_t size() const {
  // 知识点 1, shared_lock
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

```

## 逻辑上 const 而部分成员非 const：mutable

我们定义了一个const属性的 size 函数。

> const修饰函数参数是它最广泛的一种用途，它表示函数体中不能修改参数的值(包括参数本身的值或者参数其中包含的值)

我们要为了支持 mutex 而放弃声明 size() 为 const 吗？

不必，size() 在逻辑上仍是 const 的。

因此，为了让 this 为 const 时仅仅给 m_mtx 开后门，可以用 mutable 关键字修饰他，

从而所有成员里只有他不是 const 的。

## 读写锁: std::shared_mutex

因为 arr 作为可读写的共享资源，在保证安全又保证使用性能的情况下，最好能做到 "读可以共享，写必须独占，且写和读不能共存"

针对这种更具体的情况， 先贤发明了读写锁，他允许的状态有：

- n个人读取，没有人写入。
- 1个人写入，没有人读取。
- 没有人读取，也没有人写入。

cpp 标准提供了 std::shared_mutex ,

上锁时，要指定你的需求是"写"还是"读"，负责调度的读写锁会帮你判断要不要等待。

这里 push_back() 需要修改数据，因需求此为 "写"，使用 lock() 和 unlock() 的组合。
而 size() 则只要读取数据，不修改数据，因此可以和别人共享一起"读"，
使用 lock_shared() 和 unlock_shared() 的组合。

就像下面这样

```c++

class MTVector {
  std::vector<int> m_arr;
  mutable std::shared_mutex m_mtx;

public:
  void push_back(int val) {
    // std::lock_guard lock(m_mtx);
    m_mtx.lock();
    m_arr.push_back(val);
    m_mtx.unlock();
  }

  size_t size() const {
    // std::shared_lock grd(m_mtx);
    m_mtx.lock_shared();
    return m_arr.size();
    m_mtx.unlock_shared();
  }
};


```

## std::shared_lock：符合 RAII 思想的 lock_shared()

正如 std::unique_lock 针对 lock()，也可以用 std::shared_lock 针对 lock_shared()。这样就可以在函数体退出时自动调用 unlock_shared()，更加安全了。

shared_lock 同样支持 defer_lock 做参数，owns_lock() 判断等，同学们自己研究。

最上面的最终代码正是用了这种形式。
