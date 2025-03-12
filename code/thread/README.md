# Modern C++'s multi thread

## std::thread

C++11 开始，为多线程提供了语言级别的支持。他用 std::thread 这个类来表示线程。

std::thread 构造函数的参数可以是任意 lambda 表达式。

当那个线程启动时，就会执行这个 lambda 里的内容。

作为一个 C++ 类，std::thread 同样遵循 `RAII` 思想和三五法则：因为管理着资源，他自定义了解构函数，删除了拷贝构造/赋值函数，但是提供了移动构造/赋值函数。
也就是说在默认情况下，如果一个 std::thread 对象的生命周期结束了，线程也会跟着结束。

```C++
#include <chrono>
#include <iostream>
#include <string>
#include <thread>

void download(std::string file) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%) ... "
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << "\n";
}

void myfunc() {
  std::thread t1([&] { download("hello.zip"); });
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}
int main() {
  myfunc();
  interact();
  return 0;
}

```

> t1 会随着函数mufunc执行结束而结束

## 分离 std::thread::detach

解决方案：调用成员函数 detach() 分离该线程——意味着线程的生命周期不再由当前 std::thread 对象管理，而是在线程退出以后自动销毁自己。

```c++
void myfunc() {
  std::thread t1([&] { download("hello.zip"); });
+ t1.detach();
}
```

## 全局管理生命周期

也可以通过将t1变量全局保存的方式避免线程被过早销毁

```c++
+ std::vector<std::thread> m_pool;

void myfunc() {
  std::thread t1([&] { download("hello.zip"); });
+ m_pool.push_back(std::move(t1));
}

```

## 同步 std::thread::join

这个方法的作用是，让主线程阻塞等待等子线程完成。用于同步场景。

## custom threadpool

可以将上面的思想糅合起来，写一个线程池。

```c++
+ class ThreadPool {
+         std::vector<std::thread> m_pool;
+ public:
+     void push_back(std:: thread thr){
+         m_pool.push_back(std::move(thr));
+     }
+
+
+     ~ThreadPool() {
+         for (auto &t: m_pool) t.join();
+     }
+ };


+ ThreadPool pool;

void myfunc() {
  std::thread t1([&] { download("hello.zip"); });
+ pool.push_back(std::move(t1));
}


```
