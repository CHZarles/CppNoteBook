# 异步好帮手 std::async

看这个代码,类似代码也出现在 [std::thread](../thread/README.md) 的主题笔记

```c++
#include <chrono>
#include <future>
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
  return 404;
}

void interact() {
  std::string name;
  std::cin >> name;
  std::cout << "Hi, " << name << std::endl;
}

int main() {
  std::future<int> fret = std::async([&] { return download("hello.zip"); });
  interact();
  int ret = fret.get();
  std::cout << "Download returned: " << ret << std::endl;
  return 0;
}
```

- std::async 接受一个带返回值的 lambda，自身返回一个 std::future 对象。std::future 是 C++11 引入的一个类模板，用于表示异步操作的结果。它提供了一种机制，可以在异步操作完成后获取其结果。
- lambda 的函数体将在另一个线程里执行。
- 接下来你可以在 main 里面做一些别的事情，download 会持续在后台悄悄运行。
- 最后调用 future 的 get() 方法，如果此时 download 还没完成，会等待 download 完成，并获取 download 的返回值。

## 主动等待 wait() / wait_for() / wait_until()

除了 get() 会等待线程执行完毕外，wait() 也可以等待他执行完，但是不会返回其值。

```C++
int main() {
  std::future<int> fret = std::async([&] { return download("hello.zip"); });
  interact();
+ fret.wait()
  ....
  return 0;
}

```

只要线程没有执行完，wait() 会无限等下去。

而 wait_for() 则可以指定一个最长等待时间，用 chrono 里的类表示单位。他会返回一个 std::future_status 表示等待是否成功。

- 如果超过这个时间线程还没有执行完毕，则放弃等待，返回 future_status::timeout。
- 如果线程在指定的时间内执行完毕，则认为等待成功，返回 future_status::ready。

```c++
int main(){
    std::future<int> fret = std::async([&] { return download("hello.zip"); });
    interact();
    while (true) {
      std::cout << "Waiting for download to finish...\n";
      auto stat = fret.wait_for(std::chrono::milliseconds(500));
      if (stat == std::future_status::ready) {
        int ret = fret.get();
        std::cout << "Download returned: " << ret << std::endl;
        break;
      } else {
        std::cout << "Download still in progress...\n";
      }
    }
}
```

同理还有 wait_until() 其参数是一个**时间点**

## 惰性计算/延后执行

std::async 的第一个参数可以设为 std::launch::deferred，这时不会创建一个线程来执行，他只会把 lambda 函数体内的运算推迟到 future 的 get() 被调用时。也就是 main 中的 interact 计算完毕后。
这种写法，download 的执行仍在主线程中，他只是函数式编程范式意义上的异步，而不涉及到真正的多线程。可以用这个实现惰性求值（lazy evaluation）之类。

```c++
int main(){
    std::future<int> fret = std::async(std::launch::deferred,
                                       [&] { return download("hello.zip"); });
    interact();
    int ret = fret.get();
    std::cout << "Download returned: " << ret << std::endl;
}
```
