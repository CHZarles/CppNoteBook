# std::async 的底层实现: std::promise

> 知识回顾 [std::async](../async)

`std::promise` 是 C++11 引入的一个类模板，用于在多线程编程中设置异步操作的结果。它与 `std::future` 紧密相关，`std::promise` 用于设置值，而 `std::future` 用于获取值。

- 如果不想让 std::async 帮你自动创建线程，想要手动创建线程，可以直接用 std::promise。
- 然后在线程返回的时候，用 set_value() 设置返回值。在主线程里，用 get_future() 获取其 std::future 对象，进一步 get() 可以等待并获取线程返回值

```c++
#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>

int download(std::string file) {
  for (int i = 0; i < 10; ++i) {
    std::cout << "Downloading " << file << " (" << i * 10 << "%) ... "
              << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(400));
  }
  std::cout << "Download complete: " << file << "\n";
  return 404;
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}

int main() {

    // 注释 std::future<int> fret = std::async([&] { return download("hello.zip"); });
    // 等价注释部分
    std::promise<int> pret;
    std::thread t1([&] {)
      int ret = download("hello.zip");
      pret.set_value(ret);
    });
    std::future<int> fret = pret.get_future();

    interact();
    int ret = fret.get();
    std::cout << "Download returned: " << ret << std::endl;

    // 别忘记这个
    t1.join();
}
```
